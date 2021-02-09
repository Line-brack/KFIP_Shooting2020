#pragma once

//head->...->tailの末尾にnodeを追加
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//nodeをhead-tailのリストから消す(nodeの次のノードを返す)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tailのリストのノードをすべて消去
template<typename T>
void deleteAllNode(T **head, T **tail);
