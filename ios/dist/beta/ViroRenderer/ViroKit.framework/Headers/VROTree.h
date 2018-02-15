//
//  VROTree.h
//  ViroKit
//
//  Created by Raj Advani on 8/1/17.
//  Copyright © 2017 Viro Media. All rights reserved.
//

#ifndef VROTree_h
#define VROTree_h

#include <vector>
#include <functional>

template< typename T >
struct tree {
public:
    
    T value;
    std::vector<tree> children;
    
    tree() {}
    tree(T val) : value(val) {}
    
    void walkTree(std::function<void(T)> action) {
        action(value);
        for (tree &node : children) {
            node.walkTree(action);
        }
    }
    
};

#endif /* VROTree_h */
