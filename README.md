# AG News classifier using GZIP as Kolmogorov approximation


## intro:

- It uses lenght of GZIP of the text to compute distance measure.
- Distance function is multithreaded. 

- Followed this paper: [Paper on ArxViv](https://arxiv.org/abs/2212.09410)

- Used this dataset: [Dataset Ag News](https://www.kaggle.com/datasets/amananandrai/ag-news-classification-dataset/data)   

## Build

Disclaimer: Build works only on linux and posix systems

1. **The data :** download the dataset into `rootdir/resources`. There should be 2 files:
    ```
    test.csv
    train.csv
    ```
2. **Ninja :** executable will be in `~/rootdir/Bin/src/app/KNN_APP`

```bash
$~/rootdir: cmake -GNinja -S./ -BBin
$~/rootdir: cd Bin
$~/rootdir/Bin: ninja 
[6/6] Linking C executable src/app/KNN_APP
```
3. **Unix MakeFile :** executable will be in `~/rootdir/Bin/src/app/KNN_APP`
```
$~/rootdir: cmake -S./ -BBin
$~/rootdir: cd Bin
$~/rootdir/Bin: make 

```
4. **VS Code :**
- Open workspace file in `workspace.code-workspace`
- install reccomended extensions
- use **CMakeTools** extension
