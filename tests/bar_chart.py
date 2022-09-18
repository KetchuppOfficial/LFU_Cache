import matplotlib.pyplot as plt
import numpy as np
import sys

def build_bar_chart (n_tests: int):

    cat_par = [f"{i + 1}" for i in range(n_tests)]

    lfu = []
    belady = []
    for test_i in range (n_tests):

        with open(f"lfu/result_{test_i}.txt", "r") as lfu_res:
            lfu.append(int(lfu_res.readline ()))

        with open(f"belady/result_{test_i}.txt", "r") as belady_res:
            belady.append(int(belady_res.readline ()))

    x = np.arange(len(cat_par))

    plt.figure (figsize = (16,9), dpi = 80)

    width = 0.3
    plt.bar(x - width/2, lfu, width,    label = "lfu")
    plt.bar(x + width/2, belady, width, label = "belady")

    plt.title("Comparison of LFU and Belady's cache replacement algorithms")
    plt.legend()

    plt.xlabel ("Test number",    fontsize = 16)
    plt.ylabel ("Number of hits", fontsize = 16)

    plt.xticks(x)

    plt.savefig ("algorithm_comparison.png")

def main ():
    
    n_tests: int = int (sys.argv[1])
    build_bar_chart (n_tests)

main ()
