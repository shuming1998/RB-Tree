#!/bin/bash

set -e

if [ ! -d `pwd`/bin ]; then
    mkdir `pwd`/bin
fi

cd rb_tree
g++ rbtree.cpp test_rbtree.cpp -I./ -o test_rbtree
mv test_rbtree ../bin
cd ..

cd hash_table
g++ test_hashtable.cpp -I./ -o test_hashtable
mv test_hashtable ../bin
cd ..