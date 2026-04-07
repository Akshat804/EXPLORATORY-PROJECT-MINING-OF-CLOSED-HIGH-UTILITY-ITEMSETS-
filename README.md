# Mining of Closed High Utility Itemsets

## Overview
This repository contains the C++ implementation and performance analysis of five High-Utility Itemset Mining (HUIM) algorithms. High-Utility Itemset Mining extends traditional frequent pattern mining by considering both the internal quantities of items within a transaction and their external unit profits, aiming to discover itemsets that yield the highest overall utility.

This project evaluates the computational efficiency, peak memory consumption, and candidate generation overhead of traditional level-wise algorithms versus modern closed-itemset discovery techniques.

## Implemented Algorithms
1. **AprioriHC:** A level-wise candidate generation approach utilizing the Transaction-Weighted Utilization (TWU) downward closure property.
2. **AprioriHC-D:** An optimized variant of AprioriHC that incorporates transaction database reduction after each scan.
3. **CHUD (Closed High Utility Itemset Discovery):** A phase-based approach that mines a concise, lossless representation of high-utility itemsets.
4. **CloHUI (Closed High Utility Itemsets):** An efficient algorithm that avoids explicit candidate generation by utilizing in-memory utility-list structures and depth-first search.
5. **CLSMiner (Closed + Subsume):** A highly optimized approach that leverages Estimated Utility Co-occurrence Structures (EUCS) to aggressively prune the search space through early subsumption.

## Datasets
The algorithms are evaluated against two primary datasets:
* **Small Grocery:** A manually constructed verification dataset containing 20 dense transactions.
* **Large Synthetic Retail:** A synthetically generated, sparse dataset comprising 20,000 transactions across 100+ unique retail items (Electronics, Software, Home Goods) with randomized utilities. *(Data generation script included in `/scripts`)*.

## Prerequisites
* C++11 or higher
* GCC Compiler (or equivalent)

## How to Run
1. Clone the repository:
   ```bash
   git clone [https://github.com/yourusername/Closed-High-Utility-Itemset-Mining.git](https://github.com/yourusername/Closed-High-Utility-Itemset-Mining.git)
   cd Closed-High-Utility-Itemset-Mining
