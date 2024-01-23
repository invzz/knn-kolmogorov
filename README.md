# AG News classifier using GZIP as Kolmogorov approximation


## intro:



- It uses lenght of GZIP of the text to compute distance measure.
- Distance function is multithreaded. 

- [Paper on ArxViv](https://arxiv.org/abs/2212.09410)

- [Dataset Ag News](https://www.kaggle.com/datasets/amananandrai/ag-news-classification-dataset/data)   

## Build

Disclaimer: Build works only on linux and posix systems

1. **Ninja** executable will be in `~/rootdir/Bin/src/app/KNN_APP`

```bash
$~/rootdir: cmake -GNinja -S./ -BBin
$~/rootdir: cd Bin
$~/rootdir/Bin: ninja 
[6/6] Linking C executable src/app/KNN_APP
```

2. **Unix MakeFile** executable will be in `~/rootdir/Bin/src/app/KNN_APP`
```
$~/rootdir: cmake -S./ -BBin
$~/rootdir: cd Bin
$~/rootdir/Bin: make 

```

3. VS Code 
- open workspace
- install reccomended extensions

