#!/usr/bin/env python3
# graphs.py  –  compare plain multigrid with FMG start-up

import pandas as pd, matplotlib.pyplot as plt, numpy as np
from pathlib import Path
plt.rcParams.update({'lines.markersize':6, 'lines.linewidth':1.4})

# ------------------------------------------------------------------
#  helper: read the 5-column data file into a tidy dataframe
# ------------------------------------------------------------------
COLUMNS = ['lev','n','it','err','rel']         # expected column order

def load(fname:str, label:str):
    f = Path(fname)
    if not f.exists():
        raise FileNotFoundError(f'{fname} not found')
    df = pd.read_csv(f, delim_whitespace=True, names=COLUMNS)
    df['scheme'] = label                     # tag rows with MG / FMG
    return df

df_mg  = load('results.dat',      'MG')
df_fmg = load('results_fmg.dat',   'FMG')
df_all = pd.concat([df_mg, df_fmg], ignore_index=True)

# ------------------------------------------------------------------
#  functions to compute iconv and ieps from one dataframe
# ------------------------------------------------------------------
TOL   = 0.01     # 1 % plateau detector
EPS   = 1e-10

def iconv(group):
    e_prev = group['err'].iloc[0]
    for k, e in zip(group['it'][1:], group['err'][1:]):
        if abs(e-e_prev)/e_prev < TOL:
            return k
        e_prev = e
    return group['it'].iloc[-1]

def ieps(group):
    below = group[group['rel'] <= EPS]
    return below['it'].iloc[0] if not below.empty else np.nan

def conv_table(df):
    t = (df.groupby('n', as_index=False)
            .apply(lambda g: pd.Series({'iconv':iconv(g),
                                        'ieps': ieps(g)})))
    return t

tbl_mg  = conv_table(df_mg )
tbl_fmg = conv_table(df_fmg)

# ------------------------------------------------------------------
#  PLOT 0a / 0b – error history for each scheme
# ------------------------------------------------------------------
def plot_error_history(df, title, outfile):
    fig, ax = plt.subplots(figsize=(8,4))
    for lev, grp in df.groupby('lev'):
        ax.semilogy(grp['it'], grp['err'], marker='o', label=f'ℓ={lev}')
    ax.set_xlabel('iteration'); ax.set_ylabel('discrete L₂ error')
    ax.set_title(title); ax.legend(ncol=4, fontsize='small')
    ax.grid(True, which='both', ls='--', alpha=.3)
    fig.tight_layout(); fig.savefig(outfile, dpi=150)

plot_error_history(df_mg,  'Error vs iteration  (plain MG)',  'error_history_mg.png')
plot_error_history(df_fmg, 'Error vs iteration  (FMG start-up)', 'error_history_fmg.png')

# ------------------------------------------------------------------
#  PLOT 1 – i_conv  vs n  (MG & FMG)
# ------------------------------------------------------------------
fig1, ax1 = plt.subplots(figsize=(6,4))
ax1.semilogx(tbl_mg['n'],  tbl_mg['iconv'],  marker='o', label='MG')
ax1.semilogx(tbl_fmg['n'], tbl_fmg['iconv'], marker='s', label='FMG')
ax1.set_xlabel('unknowns  n')
ax1.set_ylabel(r'$i_{\mathrm{conv}}$')
ax1.set_title('Iterations to error convergence')
ax1.grid(True, which='both', ls='--', alpha=.3)
ax1.legend()
fig1.tight_layout(); fig1.savefig('iconv_vs_n.png', dpi=150)

# ------------------------------------------------------------------
#  PLOT 2 – i_eps  vs n  (MG & FMG)
# ------------------------------------------------------------------
fig2, ax2 = plt.subplots(figsize=(6,4))
ax2.semilogx(tbl_mg['n'],  tbl_mg['ieps'],  marker='d', label='MG',  color='tab:red')
ax2.semilogx(tbl_fmg['n'], tbl_fmg['ieps'], marker='^', label='FMG', color='tab:purple')
ax2.set_xlabel('unknowns  n')
ax2.set_ylabel(r'$i_\epsilon$  (ε = %.0e)' % EPS)
ax2.set_title('Iterations to reach residual ≤ ε')
ax2.grid(True, which='both', ls='--', alpha=.3)
ax2.legend()
fig2.tight_layout(); fig2.savefig('ieps_vs_n.png', dpi=150)

# ------------------------------------------------------------------
#  optional combined two-panel summary
# ------------------------------------------------------------------
figc, (axc1, axc2) = plt.subplots(1,2, figsize=(10,4))
axc1.semilogx(tbl_mg['n'],  tbl_mg['iconv'],  'o-', label='MG')
axc1.semilogx(tbl_fmg['n'], tbl_fmg['iconv'], 's-', label='FMG')
axc1.set_xlabel('n'); axc1.set_ylabel(r'$i_{\mathrm{conv}}$'); axc1.set_title('error plateau')
axc1.grid(True, which='both', ls='--', alpha=.3)

axc2.semilogx(tbl_mg['n'],  tbl_mg['ieps'],  'd-', label='MG')
axc2.semilogx(tbl_fmg['n'], tbl_fmg['ieps'], '^-', label='FMG')
axc2.set_xlabel('n'); axc2.set_ylabel(r'$i_\epsilon$'); axc2.set_title('residual ≤ ε')
axc2.grid(True, which='both', ls='--', alpha=.3)

axc1.legend(); axc2.legend()
figc.tight_layout(); figc.savefig('compare_iconv_ieps.png', dpi=150)

print('Plots written:\n'
      '  error_history_mg.png / error_history_fmg.png\n'
      '  iconv_vs_n.png   ieps_vs_n.png\n'
      '  compare_iconv_ieps.png')
