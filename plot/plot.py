import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import re
import os
import numpy as np

# CONFIG

RESULTS_FILE = 'plot/results.txt'
OUTPUT_DIR = 'plot/charts'
PLOT_STYLE = 'seaborn-v0_8-whitegrid'

# PARSING

def parse_time(time_str):
    time_str = time_str.strip()
    if ' ns' in time_str:
        return float(time_str.replace(' ns', ''))
    elif ' ms' in time_str:
        return float(time_str.replace(' ms', '')) * 1_000_000
    elif ' s' in time_str:
        return float(time_str.replace(' s', '')) * 1_000_000_000
    else:
        try:
            return float(time_str)
        except ValueError:
            return None

def parse_benchmark_name(name):
    parts = name.split('/')
    base_name = parts[0]
    size_n = int(parts[1]) if len(parts) > 1 else None

    tree_type = None
    operation = None
    alpha = None

    if base_name.startswith('BM_AVL'):
        tree_type = 'AVL'
        operation = base_name.replace('BM_AVL_', '')
    elif base_name.startswith('BM_Scapegoat_AlphaTuning'):
        tree_type = 'Scapegoat'
        alpha_match = re.search(r'_(\d+)$', base_name)
        if alpha_match:
            alpha = int(alpha_match.group(1)) / 100.0
            operation = f'AlphaTuning_{alpha}'
        else:
            operation = 'AlphaTuning_Unknown'
    elif base_name.startswith('BM_Scapegoat'):
        tree_type = 'Scapegoat'
        operation = base_name.replace('BM_Scapegoat_', '')
        alpha = 0.7

    return tree_type, operation, size_n, alpha


def load_data(filepath):
    data = []
    with open(filepath, 'r') as f:
        lines = f.readlines()

    for line in lines[1:]:
        line = line.strip()
        if not line or line.startswith('-'):
            continue

        parts = re.split(r'\s{2,}', line)
        if len(parts) < 3:
            print(f"Skipping malformed line: {line}")
            continue

        benchmark_full_name = parts[0]
        time_str = parts[1]
        cpu_time_str = parts[2]
        # iterations = parts[3]

        tree_type, operation, size_n, alpha = parse_benchmark_name(benchmark_full_name)
        time_ns = parse_time(time_str)
        cpu_time_ns = parse_time(cpu_time_str)

        if tree_type and operation and size_n is not None and time_ns is not None:
            data.append({
                'Benchmark': benchmark_full_name,
                'TreeType': tree_type,
                'Operation': operation,
                'Size': size_n,
                'Alpha': alpha,
                'Time_ns': time_ns,
                'CPUTime_ns': cpu_time_ns
            })
        else:
             print(f"Skipping line due to parsing error: {line}")


    return pd.DataFrame(data)

# PLOTTING

def plot_comparison(df, operations, title, filename, y_col='Time_ns', y_label='Time (ns)', log_y=True):
    plt.style.use(PLOT_STYLE)
    fig, ax = plt.subplots(figsize=(12, 7))

    plot_df = df[df['Operation'].isin(operations)].copy()

    plot_df['Size'] = pd.to_numeric(plot_df['Size'])
    plot_df = plot_df.sort_values(by='Size')

    sns.lineplot(data=plot_df, x='Size', y=y_col, hue='TreeType', style='Operation', marker='o', ax=ax)

    ax.set_title(title, fontsize=16)
    ax.set_xlabel('Input Size (n)', fontsize=12)
    ax.set_ylabel(y_label, fontsize=12)
    ax.set_xscale('log', base=2)
    if log_y:
        ax.set_yscale('log')

    ax.legend(title='Tree Type / Operation', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.tight_layout(rect=[0, 0, 0.85, 1])

    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    filepath = os.path.join(OUTPUT_DIR, filename)
    plt.savefig(filepath)
    print(f"Saved plot: {filepath}")
    plt.close(fig)

def plot_alpha_tuning(df, filename, y_col='Time_ns', y_label='Time (ns)', log_y=True):
    plt.style.use(PLOT_STYLE)
    fig, ax = plt.subplots(figsize=(12, 7))

    plot_df = df[df['Operation'].str.startswith('AlphaTuning_')].copy()
    if plot_df.empty:
        print("No AlphaTuning data found to plot.")
        plt.close(fig)
        return

    plot_df['Size'] = pd.to_numeric(plot_df['Size'])
    plot_df = plot_df.sort_values(by=['Alpha', 'Size'])

    sns.lineplot(data=plot_df, x='Size', y=y_col, hue='Alpha', palette='viridis', marker='o', ax=ax)

    ax.set_title('Scapegoat Tree Performance by Alpha Value (Random Insert)', fontsize=16)
    ax.set_xlabel('Input Size (n)', fontsize=12)
    ax.set_ylabel(y_label, fontsize=12)
    ax.set_xscale('log', base=2)
    if log_y:
        ax.set_yscale('log')

    ax.legend(title='Alpha Value', bbox_to_anchor=(1.05, 1), loc='upper left')
    plt.xticks(fontsize=10)
    plt.yticks(fontsize=10)
    plt.tight_layout(rect=[0, 0, 0.85, 1])

    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    filepath = os.path.join(OUTPUT_DIR, filename)
    plt.savefig(filepath)
    print(f"Saved plot: {filepath}")
    plt.close(fig)


# MAIN

if __name__ == "__main__":
    print(f"Loading data from {RESULTS_FILE}...")
    df_results = load_data(RESULTS_FILE)

    if df_results.empty:
        print("No data loaded. Exiting.")
    else:
        print("Data loaded successfully:")
        print(df_results.head())
        print(f"\nUnique Operations: {df_results['Operation'].unique()}")
        print(f"Unique Tree Types: {df_results['TreeType'].unique()}")

        # 1. Insertion Comparison (Sequential Ascending, Descending, Random, Mixed)
        insertion_ops = [
            'SequentialInsertAscending', 'SequentialInsertDescending',
            'RandomInsert', 'MixedPatternInsert'
        ]
        plot_comparison(df_results, insertion_ops,
                        'Insertion Performance: AVL vs. Scapegoat',
                        'insertion_comparison.png')

        # 2. Deletion Comparison (Random, Sequential, Delete-Heavy)
        deletion_ops = [
            'RandomDeletion', 'SequentialDeletion', 'DeleteHeavyWorkload'
        ]
        plot_comparison(df_results, deletion_ops,
                        'Deletion Performance: AVL vs. Scapegoat',
                        'deletion_comparison.png')

        # 3. Search Comparison (Successful, Unsuccessful, Distribution)
        search_ops = [
            'SuccessfulSearch', 'UnsuccessfulSearch', 'SearchDistribution'
        ]
        plot_comparison(df_results, search_ops,
                        'Search Performance: AVL vs. Scapegoat',
                        'search_comparison.png')

        # 4. Range Query Comparison (Small, Large, Empty)
        range_query_ops = [
            'SmallRangeQuery', 'LargeRangeQuery', 'EmptyRangeQuery'
        ]
        plot_comparison(df_results, range_query_ops,
                        'Range Query Performance: AVL vs. Scapegoat',
                        'range_query_comparison.png')

        # 5. Mixed Workload Comparison (Dictionary, Database Index)
        mixed_workload_ops = [
            'DictionaryOperations', 'DatabaseIndex'
        ]
        plot_comparison(df_results, mixed_workload_ops,
                        'Mixed Workload Performance: AVL vs. Scapegoat',
                        'mixed_workload_comparison.png')

        # 6. Worst Case Comparison
        worst_case_ops = ['WorstCase']
        plot_comparison(df_results, worst_case_ops,
                        'Worst Case Performance: AVL vs. Scapegoat',
                        'worst_case_comparison.png')

        # 7. Large Dataset Comparison (Using ms unit might be better here)
        large_data_ops = ['LargeDataset']
        # Create a temporary df with time in ms for this plot
        df_large = df_results[df_results['Operation'].isin(large_data_ops)].copy()
        df_large['Time_ms'] = df_large['Time_ns'] / 1_000_000
        plot_comparison(df_large, large_data_ops,
                        'Large Dataset Performance: AVL vs. Scapegoat',
                        'large_dataset_comparison.png',
                        y_col='Time_ms', y_label='Time (ms)', log_y=False) # Often better linear for ms

        # 8. Scapegoat Alpha Tuning Comparison
        plot_alpha_tuning(df_results, 'scapegoat_alpha_tuning.png')

        print(f"\nAll plots saved to {OUTPUT_DIR}")