#include <benchmark/benchmark.h>
#include "avl.h"
#include "scapegoat.h"
#include <random>
#include <algorithm>
#include <vector>
#include <chrono>

std::mt19937 g_rng(std::random_device{}());

// helper functions for benchmark setup
std::vector<int> generateRandomKeys(size_t n, int min = 0, int max = 1000000) {
    std::vector<int> keys(n);
    std::uniform_int_distribution<> distrib(min, max);
    
    for (size_t i = 0; i < n; ++i) {
        keys[i] = distrib(g_rng);
    }
    
    return keys;
}

// fast random key generation for large sequences
std::vector<int> generateRandomKeysLinear(size_t n, int min = 0, int max = 1000000) {
    std::vector<int> keys(n);
    
    // if the range is large enough, we can use a linear approach to avoid excessive random calls
    if (max - min + 1 >= n * 2) {
        // generate a linearly spaced sequence and shuffle it
        for (size_t i = 0; i < n; ++i) {
            keys[i] = min + static_cast<int>(i * ((max - min) / n));
        }
        std::shuffle(keys.begin(), keys.end(), g_rng);
    } else {
        // fall back to standard random generation
        std::uniform_int_distribution<> distrib(min, max);
        for (size_t i = 0; i < n; ++i) {
            keys[i] = distrib(g_rng);
        }
    }
    
    return keys;
}

std::vector<int> generateSequentialKeys(size_t n, bool ascending = true) {
    std::vector<int> keys(n);
    
    if (ascending) {
        for (size_t i = 0; i < n; ++i) {
            keys[i] = static_cast<int>(i);
        }
    } else {
        for (size_t i = 0; i < n; ++i) {
            keys[i] = static_cast<int>(n - i - 1);
        }
    }
    
    return keys;
}

std::vector<int> generateMixedPattern(size_t n) {
    std::vector<int> keys;
    keys.reserve(n);
    
    int segmentSize = static_cast<int>(std::sqrt(n));
    int currentStart = 0;
    
    while (keys.size() < n) {
        // Add ascending segment
        int segmentEnd = currentStart + segmentSize;
        for (int i = currentStart; i < segmentEnd && keys.size() < n; ++i) {
            keys.push_back(i);
        }
        
        currentStart = segmentEnd;
        
        // Add descending segment
        segmentEnd = currentStart + segmentSize;
        for (int i = segmentEnd - 1; i >= currentStart && keys.size() < n; --i) {
            keys.push_back(i);
        }
        
        currentStart = segmentEnd;
    }
    
    return keys;
}

//------------------------------------------------------------------
// 1. INSERTION BENCHMARKS
//------------------------------------------------------------------

// Sequential Insertion: Insert keys in ascending or descending order
static void BM_AVL_SequentialInsertAscending(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateSequentialKeys(n, true);
        AVLTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_SequentialInsertAscending)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SequentialInsertAscending(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateSequentialKeys(n, true);
        ScapegoatTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_SequentialInsertAscending)->Range(8, 8<<10)->Threads(8);

static void BM_AVL_SequentialInsertDescending(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateSequentialKeys(n, false);
        AVLTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_SequentialInsertDescending)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SequentialInsertDescending(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateSequentialKeys(n, false);
        ScapegoatTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_SequentialInsertDescending)->Range(8, 8<<10)->Threads(8);

static void BM_AVL_RandomInsert(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_RandomInsert)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_RandomInsert(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_RandomInsert)->Range(8, 8<<10)->Threads(8);

static void BM_AVL_MixedPatternInsert(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateMixedPattern(n);
        AVLTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_MixedPatternInsert)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_MixedPatternInsert(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateMixedPattern(n);
        ScapegoatTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_MixedPatternInsert)->Range(8, 8<<10)->Threads(8);

//------------------------------------------------------------------
// 2. DELETION BENCHMARKS
//------------------------------------------------------------------

// Random Deletion: randomly delete elements
static void BM_AVL_RandomDeletion(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // shuffle keys for random deletion order
        std::shuffle(keys.begin(), keys.end(), g_rng);
        state.ResumeTiming();
        
        // delete all keys in random order
        for (int key : keys) {
            tree.remove(key);
        }
    }
}
BENCHMARK(BM_AVL_RandomDeletion)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_RandomDeletion(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // shuffle keys for random deletion order
        std::shuffle(keys.begin(), keys.end(), g_rng);
        state.ResumeTiming();
        
        // delete all keys in random order
        for (int key : keys) {
            tree.remove(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_RandomDeletion)->Range(8, 8<<9)->Threads(8);

// Sequential Deletion: delete elements in order (forward)
static void BM_AVL_SequentialDeletion(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // sort keys for sequential deletion
        std::sort(keys.begin(), keys.end());
        state.ResumeTiming();
        
        // delete all keys in sequential order
        for (int key : keys) {
            tree.remove(key);
        }
    }
}
BENCHMARK(BM_AVL_SequentialDeletion)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SequentialDeletion(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // sort keys for sequential deletion
        std::sort(keys.begin(), keys.end());
        state.ResumeTiming();
        
        // delete all keys in sequential order
        for (int key : keys) {
            tree.remove(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_SequentialDeletion)->Range(8, 8<<9)->Threads(8);

// Delete-Heavy Workload: many deletions with few insertions
// highlight Scapegoat's rebuild cost
static void BM_AVL_DeleteHeavyWorkload(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // select 80% of keys for deletion
        size_t deleteCount = (n * 4) / 5;
        std::vector<int> keysToDelete(keys.begin(), keys.begin() + deleteCount);
        // generate some new keys to insert (20% of original size)
        std::vector<int> newKeys = generateRandomKeysLinear(n / 5, 1000001, 2000000);
        state.ResumeTiming();
        
        // delete 80% of keys
        for (int key : keysToDelete) {
            tree.remove(key);
        }
        
        // insert 20% new keys
        for (int key : newKeys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_DeleteHeavyWorkload)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_DeleteHeavyWorkload(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // select 80% of keys for deletion
        size_t deleteCount = (n * 4) / 5;
        std::vector<int> keysToDelete(keys.begin(), keys.begin() + deleteCount);
        // generate some new keys to insert (20% of original size)
        std::vector<int> newKeys = generateRandomKeysLinear(n / 5, 1000001, 2000000);
        state.ResumeTiming();
        
        // delete 80% of keys
        for (int key : keysToDelete) {
            tree.remove(key);
        }
        
        // insert 20% new keys
        for (int key : newKeys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_DeleteHeavyWorkload)->Range(8, 8<<9)->Threads(8);

//------------------------------------------------------------------
// 3. SEARCH BENCHMARKS
//------------------------------------------------------------------

// Successful Search: find elements known to be in the tree
static void BM_AVL_SuccessfulSearch(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // shuffle keys for random search order
        std::shuffle(keys.begin(), keys.end(), g_rng);
        
        // take 20% of keys for search
        size_t searchCount = n / 5;
        std::vector<int> searchKeys(keys.begin(), keys.begin() + searchCount);
        state.ResumeTiming();
        
        // search for keys (all should be found)
        for (int key : searchKeys) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_AVL_SuccessfulSearch)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SuccessfulSearch(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        // shuffle keys for random search order
        std::shuffle(keys.begin(), keys.end(), g_rng);
        
        // take 20% of keys for search
        size_t searchCount = n / 5;
        std::vector<int> searchKeys(keys.begin(), keys.begin() + searchCount);
        state.ResumeTiming();
        
        // search for keys (all should be found)
        for (int key : searchKeys) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_Scapegoat_SuccessfulSearch)->Range(8, 8<<10)->Threads(8);

// Unsuccessful Search: Search for elements not in the tree
static void BM_AVL_UnsuccessfulSearch(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // generate keys that are not in the tree
        std::vector<int> nonExistingKeys = generateRandomKeysLinear(n / 5, 1000001, 2000000);
        state.ResumeTiming();
        
        // search for non-existing keys
        for (int key : nonExistingKeys) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_AVL_UnsuccessfulSearch)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_UnsuccessfulSearch(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // generate keys that are not in the tree
        std::vector<int> nonExistingKeys = generateRandomKeysLinear(n / 5, 1000001, 2000000);
        state.ResumeTiming();
        
        // search for non-existing keys
        for (int key : nonExistingKeys) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_Scapegoat_UnsuccessfulSearch)->Range(8, 8<<10)->Threads(8);

// Search Distribution: test search performance based on key distribution
// test depth-based search in balanced vs slightly imbalanced trees
static void BM_AVL_SearchDistribution(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        // create skewed data where 80% of keys are in a narrow range
        // and 20% are spread wider
        std::vector<int> keys;
        keys.reserve(n);
        
        // 80% of keys in narrow range [0, 1000]
        size_t narrowCount = (n * 4) / 5;
        std::vector<int> narrowKeys = generateRandomKeysLinear(narrowCount, 0, 1000);
        keys.insert(keys.end(), narrowKeys.begin(), narrowKeys.end());
        
        // 20% of keys in wider range [1001, 1000000]
        std::vector<int> wideKeys = generateRandomKeysLinear(n - narrowCount, 1001, 1000000);
        keys.insert(keys.end(), wideKeys.begin(), wideKeys.end());
        
        AVLTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // create search keys with same distribution
        std::vector<int> searchKeysNarrow = generateRandomKeysLinear(100, 0, 1000);
        std::vector<int> searchKeysWide = generateRandomKeysLinear(100, 1001, 1000000);
        state.ResumeTiming();
        
        // search in narrow range (higher probability of success)
        for (int key : searchKeysNarrow) {
            benchmark::DoNotOptimize(tree.search(key));
        }
        
        // search in wide range (lower probability of success)
        for (int key : searchKeysWide) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_AVL_SearchDistribution)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SearchDistribution(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        // create skewed data where 80% of keys are in a narrow range
        // and 20% are spread wider
        std::vector<int> keys;
        keys.reserve(n);
        
        // 80% of keys in narrow range [0, 1000]
        size_t narrowCount = (n * 4) / 5;
        std::vector<int> narrowKeys = generateRandomKeysLinear(narrowCount, 0, 1000);
        keys.insert(keys.end(), narrowKeys.begin(), narrowKeys.end());
        
        // 20% of keys in wider range [1001, 1000000]
        std::vector<int> wideKeys = generateRandomKeysLinear(n - narrowCount, 1001, 1000000);
        keys.insert(keys.end(), wideKeys.begin(), wideKeys.end());
        
        ScapegoatTree tree;
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // create search keys with same distribution
        std::vector<int> searchKeysNarrow = generateRandomKeysLinear(100, 0, 1000);
        std::vector<int> searchKeysWide = generateRandomKeysLinear(100, 1001, 1000000);
        state.ResumeTiming();
        
        // search in narrow range (higher probability of success)
        for (int key : searchKeysNarrow) {
            benchmark::DoNotOptimize(tree.search(key));
        }
        
        // search in wide range (lower probability of success)
        for (int key : searchKeysWide) {
            benchmark::DoNotOptimize(tree.search(key));
        }
    }
}
BENCHMARK(BM_Scapegoat_SearchDistribution)->Range(8, 8<<10)->Threads(8);

//------------------------------------------------------------------
// 4. RANGE QUERY BENCHMARKS
//------------------------------------------------------------------

// Small Range: query a small subset of the tree (5% of keys)
static void BM_AVL_SmallRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        AVLTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to know the range
        std::sort(keys.begin(), keys.end());
        
        // select a small range (approximately 5% of keys)
        size_t rangeSize = n / 20;
        size_t startIdx = n / 2 - rangeSize / 2; // Center the range
        int rangeStart = keys[startIdx];
        int rangeEnd = keys[startIdx + rangeSize - 1];
        
        state.ResumeTiming();
        
        // perform range query
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AVL_SmallRangeQuery)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_SmallRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        ScapegoatTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to know the range
        std::sort(keys.begin(), keys.end());
        
        // select a small range (approximately 5% of keys)
        size_t rangeSize = n / 20;
        size_t startIdx = n / 2 - rangeSize / 2; // Center the range
        int rangeStart = keys[startIdx];
        int rangeEnd = keys[startIdx + rangeSize - 1];
        
        state.ResumeTiming();
        
        // perform range query
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Scapegoat_SmallRangeQuery)->Range(8, 8<<10)->Threads(8);

// Large Range: query a large portion of the tree (50% of keys)
static void BM_AVL_LargeRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        AVLTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to know the range
        std::sort(keys.begin(), keys.end());
        
        // select a large range (approximately 50% of keys)
        size_t rangeSize = n / 2;
        size_t startIdx = n / 4; // start at 25% mark
        int rangeStart = keys[startIdx];
        int rangeEnd = keys[startIdx + rangeSize - 1];
        
        state.ResumeTiming();
        
        // perform range query
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AVL_LargeRangeQuery)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_LargeRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        ScapegoatTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to know the range
        std::sort(keys.begin(), keys.end());
        
        // select a large range (approximately 50% of keys)
        size_t rangeSize = n / 2;
        size_t startIdx = n / 4; // start at 25% mark
        int rangeStart = keys[startIdx];
        int rangeEnd = keys[startIdx + rangeSize - 1];
        
        state.ResumeTiming();
        
        // perform range query
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Scapegoat_LargeRangeQuery)->Range(8, 8<<10)->Threads(8);

// Empty Range: query a range with no elements
static void BM_AVL_EmptyRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        AVLTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to find gaps
        std::sort(keys.begin(), keys.end());
        
        // find a gap between keys
        int rangeStart = -1, rangeEnd = -1;
        for (size_t i = 1; i < keys.size(); ++i) {
            if (keys[i] > keys[i-1] + 1) {
                rangeStart = keys[i-1] + 1;
                rangeEnd = keys[i] - 1;
                break;
            }
        }
        
        // if no gap found, use range outside the keys
        if (rangeStart == -1) {
            rangeStart = 2000000;
            rangeEnd = 2001000;
        }
        
        state.ResumeTiming();
        
        // perform range query (should be empty)
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_AVL_EmptyRangeQuery)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_EmptyRangeQuery(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n, 0, 1000000);
        ScapegoatTree tree;
        
        // insert all keys
        for (int key : keys) {
            tree.insert(key);
        }
        
        // sort keys to find gaps
        std::sort(keys.begin(), keys.end());
        
        // find a gap between keys
        int rangeStart = -1, rangeEnd = -1;
        for (size_t i = 1; i < keys.size(); ++i) {
            if (keys[i] > keys[i-1] + 1) {
                rangeStart = keys[i-1] + 1;
                rangeEnd = keys[i] - 1;
                break;
            }
        }
        
        // if no gap found, use range outside the keys
        if (rangeStart == -1) {
            rangeStart = 2000000;
            rangeEnd = 2001000;
        }
        
        state.ResumeTiming();
        
        // perform range query (should be empty)
        std::vector<int> result = tree.rangeQuery(rangeStart, rangeEnd);
        benchmark::DoNotOptimize(result);
    }
}
BENCHMARK(BM_Scapegoat_EmptyRangeQuery)->Range(8, 8<<10)->Threads(8);

//------------------------------------------------------------------
// 7. REAL-WORLD SCENARIO
//------------------------------------------------------------------

// Dictionary Operations: mixed operations simulating a dictionary (insert/search/delete)
static void BM_AVL_DictionaryOperations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        
        // initial set of keys (50% of n)
        std::vector<int> initialKeys = generateRandomKeysLinear(n/2);
        
        // operations to perform (insert, search, delete) in mixed order
        std::vector<std::pair<int, int>> operations; // (operation, key): 0=insert, 1=search, 2=delete
        
        // generate keys for operations
        std::vector<int> insertKeys = generateRandomKeysLinear(n/4, 1000001, 2000000);
        
        // create all operations
        // 25% inserts
        for (int key : insertKeys) {
            operations.push_back({0, key});
        }
        
        // 50% searches (half existing, half non-existing)
        std::vector<int> existingKeys(initialKeys.begin(), initialKeys.begin() + n/4);
        std::vector<int> nonExistingKeys = generateRandomKeysLinear(n/4, 2000001, 3000000);
        
        for (int key : existingKeys) {
            operations.push_back({1, key});
        }
        
        for (int key : nonExistingKeys) {
            operations.push_back({1, key});
        }
        
        // 25% deletes
        std::vector<int> deleteKeys(initialKeys.begin() + n/4, initialKeys.begin() + n/2);
        for (int key : deleteKeys) {
            operations.push_back({2, key});
        }
        
        // shuffle operations
        std::shuffle(operations.begin(), operations.end(), g_rng);
        
        AVLTree tree;
        
        // insert initial keys
        for (int key : initialKeys) {
            tree.insert(key);
        }
        
        state.ResumeTiming();
        
        // perform mixed operations
        for (const auto& op : operations) {
            int operation = op.first;
            int key = op.second;
            
            switch (operation) {
                case 0: // insert
                    tree.insert(key);
                    break;
                case 1: // search
                    benchmark::DoNotOptimize(tree.search(key));
                    break;
                case 2: // delete
                    tree.remove(key);
                    break;
            }
        }
    }
}
BENCHMARK(BM_AVL_DictionaryOperations)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_DictionaryOperations(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        
        // initial set of keys (50% of n)
        std::vector<int> initialKeys = generateRandomKeysLinear(n/2);
        
        // operations to perform (insert, search, delete) in mixed order
        std::vector<std::pair<int, int>> operations; // (operation, key): 0=insert, 1=search, 2=delete
        
        // generate keys for operations
        std::vector<int> insertKeys = generateRandomKeysLinear(n/4, 1000001, 2000000);
        
        // create all operations
        // 25% inserts
        for (int key : insertKeys) {
            operations.push_back({0, key});
        }
        
        // 50% searches (half existing, half non-existing)
        std::vector<int> existingKeys(initialKeys.begin(), initialKeys.begin() + n/4);
        std::vector<int> nonExistingKeys = generateRandomKeysLinear(n/4, 2000001, 3000000);
        
        for (int key : existingKeys) {
            operations.push_back({1, key});
        }
        
        for (int key : nonExistingKeys) {
            operations.push_back({1, key});
        }
        
        // 25% deletes
        std::vector<int> deleteKeys(initialKeys.begin() + n/4, initialKeys.begin() + n/2);
        for (int key : deleteKeys) {
            operations.push_back({2, key});
        }
        
        // shuffle operations
        std::shuffle(operations.begin(), operations.end(), g_rng);
        
        ScapegoatTree tree;
        
        // insert initial keys
        for (int key : initialKeys) {
            tree.insert(key);
        }
        
        state.ResumeTiming();
        
        // perform mixed operations
        for (const auto& op : operations) {
            int operation = op.first;
            int key = op.second;
            
            switch (operation) {
                case 0: // insert
                    tree.insert(key);
                    break;
                case 1: // search
                    benchmark::DoNotOptimize(tree.search(key));
                    break;
                case 2: // delete
                    tree.remove(key);
                    break;
            }
        }
    }
}
BENCHMARK(BM_Scapegoat_DictionaryOperations)->Range(8, 8<<10)->Threads(8);

// Database Index: simulate database index operations (range queries, inserts, specific lookups)
static void BM_AVL_DatabaseIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        
        // initial set of keys (80% of n)
        std::vector<int> initialKeys = generateRandomKeysLinear(n * 4 / 5, 0, 1000000);
        
        // operations to perform: range queries, inserts, lookups
        std::vector<std::tuple<int, int, int>> operations; // (operation, param1, param2): 0=insert, 1=lookup, 2=range
        
        // generate keys for operations
        std::vector<int> insertKeys = generateRandomKeysLinear(n/10, 1000001, 2000000);
        
        // create all operations
        // 10% inserts
        for (int key : insertKeys) {
            operations.push_back({0, key, 0});
        }
        
        // 60% lookups (existing and non-existing)
        std::vector<int> lookupKeys = generateRandomKeysLinear(n * 6 / 10, 0, 2000000);
        for (int key : lookupKeys) {
            operations.push_back({1, key, 0});
        }
        
        // 30% range queries - use uniform distribution for start and offset
        std::uniform_int_distribution<> start_dist(0, 1000000);
        std::uniform_int_distribution<> range_dist(1, 50000);
        std::vector<std::pair<int, int>> rangeQueries;
        rangeQueries.reserve(n * 3 / 10);
        
        // pre-generate all range queries at once
        for (size_t i = 0; i < n * 3 / 10; ++i) {
            int start = start_dist(g_rng);
            int end = start + range_dist(g_rng);
            rangeQueries.push_back({start, end});
        }
        
        // add the range queries to operations
        for (const auto& query : rangeQueries) {
            operations.push_back({2, query.first, query.second});
        }
        
        // shuffle operations
        std::shuffle(operations.begin(), operations.end(), g_rng);
        
        AVLTree tree;
        
        // insert initial keys
        for (int key : initialKeys) {
            tree.insert(key);
        }
        
        state.ResumeTiming();
        
        // perform mixed operations
        for (const auto& op : operations) {
            int operation = std::get<0>(op);
            int param1 = std::get<1>(op);
            int param2 = std::get<2>(op);
            
            switch (operation) {
                case 0: // insert
                    tree.insert(param1);
                    break;
                case 1: // lookup
                    benchmark::DoNotOptimize(tree.search(param1));
                    break;
                case 2: // range query
                    benchmark::DoNotOptimize(tree.rangeQuery(param1, param2));
                    break;
            }
        }
    }
}
BENCHMARK(BM_AVL_DatabaseIndex)->Range(8, 8<<9)->Threads(8);

static void BM_Scapegoat_DatabaseIndex(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        
        // initial set of keys (80% of n)
        std::vector<int> initialKeys = generateRandomKeysLinear(n * 4 / 5, 0, 1000000);
        
        // operations to perform: range queries, inserts, lookups
        std::vector<std::tuple<int, int, int>> operations; // (operation, param1, param2): 0=insert, 1=lookup, 2=range
        
        // generate keys for operations
        std::vector<int> insertKeys = generateRandomKeysLinear(n/10, 1000001, 2000000);
        
        // create all operations
        // 10% inserts
        for (int key : insertKeys) {
            operations.push_back({0, key, 0});
        }
        
        // 60% lookups (existing and non-existing)
        std::vector<int> lookupKeys = generateRandomKeysLinear(n * 6 / 10, 0, 2000000);
        for (int key : lookupKeys) {
            operations.push_back({1, key, 0});
        }
        
        // 30% range queries - use uniform distribution for start and offset
        std::uniform_int_distribution<> start_dist(0, 1000000);
        std::uniform_int_distribution<> range_dist(1, 50000);
        std::vector<std::pair<int, int>> rangeQueries;
        rangeQueries.reserve(n * 3 / 10);
        
        // pre-generate all range queries at once
        for (size_t i = 0; i < n * 3 / 10; ++i) {
            int start = start_dist(g_rng);
            int end = start + range_dist(g_rng);
            rangeQueries.push_back({start, end});
        }
        
        // add the range queries to operations
        for (const auto& query : rangeQueries) {
            operations.push_back({2, query.first, query.second});
        }
        
        // shuffle operations
        std::shuffle(operations.begin(), operations.end(), g_rng);
        
        ScapegoatTree tree;
        
        // insert initial keys
        for (int key : initialKeys) {
            tree.insert(key);
        }
        
        state.ResumeTiming();
        
        // perform mixed operations
        for (const auto& op : operations) {
            int operation = std::get<0>(op);
            int param1 = std::get<1>(op);
            int param2 = std::get<2>(op);
            
            switch (operation) {
                case 0: // insert
                    tree.insert(param1);
                    break;
                case 1: // lookup
                    benchmark::DoNotOptimize(tree.search(param1));
                    break;
                case 2: // range query
                    benchmark::DoNotOptimize(tree.rangeQuery(param1, param2));
                    break;
            }
        }
    }
}
BENCHMARK(BM_Scapegoat_DatabaseIndex)->Range(8, 8<<9)->Threads(8);

//------------------------------------------------------------------
// 9. TREE-SPECIFIC TESTS
//------------------------------------------------------------------

// alpha tuning: test different alpha values to find optimal settings for scapegoat tree
static void BM_Scapegoat_AlphaTuning_60(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree(0.6); // alpha = 0.6
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_AlphaTuning_60)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_AlphaTuning_70(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree(0.7); // alpha = 0.7 (default)
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_AlphaTuning_70)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_AlphaTuning_80(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree(0.8); // alpha = 0.8
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_AlphaTuning_80)->Range(8, 8<<10)->Threads(8);

static void BM_Scapegoat_AlphaTuning_90(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree(0.9); // alpha = 0.9
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_AlphaTuning_90)->Range(8, 8<<10)->Threads(8);

//------------------------------------------------------------------
// 10. STRESS TESTS
//------------------------------------------------------------------

// create worst-case scenarios for each tree type
// worst case for avl: continuous insertions in sorted order
static void BM_AVL_WorstCase(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        AVLTree tree;
        state.ResumeTiming();
        
        // insert in sorted order (would create a right-skewed tree without balancing)
        for (size_t i = 0; i < n; ++i) {
            tree.insert(static_cast<int>(i));
        }
    }
}
BENCHMARK(BM_AVL_WorstCase)->Range(8, 8<<10)->Threads(8);

// worst case for scapegoat: insertion pattern that maximizes rebuilding
static void BM_Scapegoat_WorstCase(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        ScapegoatTree tree;
        
        // pre-compute keys that will force many rebuilds
        // pattern: insert at increasing depths to force height violations
        std::vector<int> keys;
        keys.reserve(n);
        
        // fill with a pattern that tends to create deep paths
        for (size_t i = 0; i < n; ++i) {
            if (i % 2 == 0) {
                // even indices: increasing sequence
                keys.push_back(10000 + static_cast<int>(i));
            } else {
                // odd indices: numbers between existing values
                keys.push_back(static_cast<int>(i) * 100);
            }
        }
        
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_WorstCase)->Range(8, 8<<10)->Threads(8);

// large dataset test: test with larger entries for stability and performance
static void BM_AVL_LargeDataset(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        AVLTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_AVL_LargeDataset)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond)->Threads(8);

static void BM_Scapegoat_LargeDataset(benchmark::State& state) {
    for (auto _ : state) {
        state.PauseTiming();
        size_t n = state.range(0);
        std::vector<int> keys = generateRandomKeysLinear(n);
        ScapegoatTree tree;
        state.ResumeTiming();
        
        for (int key : keys) {
            tree.insert(key);
        }
    }
}
BENCHMARK(BM_Scapegoat_LargeDataset)->Range(1<<10, 1<<14)->Unit(benchmark::kMillisecond)->Threads(8);

BENCHMARK_MAIN(); 