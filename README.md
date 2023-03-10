# Gobang

## 描述

基于alpha-beta剪枝的五子棋程序，目前能进行7层的搜索。

## 输入格式

坐标的输入格式为：**8h**

## 实现的功能

目前已实现的功能有：

- 利用KMP算法匹配棋形
- 基于哈希表记录局面信息并实时更新各局面向量的评分，实现更快的局面分计算
  - 此处哈希表使用的是`uthash`，附上github地址：[troydhanson/uthash: C macros for hash tables and more (github.com)](https://github.com/troydhanson/uthash)
- αβ剪枝
- PVS主要变例搜索
- 散列表&zobrist散列
- 迭代加深
- 基于历史表的启发式搜索

## 待实现的功能

- [ ] MTD(f)搜索
- [ ] vcf&vct
- [ ] 禁手
- [ ] 更好的评分策略
- [ ] 多线程计算