#pragma once

//head->...->tail�̖�����node��ǉ�
template<typename T>
void pushBack(T **node, T ** head, T **tail);
//node��head-tail�̃��X�g�������(node�̎��̃m�[�h��Ԃ�)
template<typename T>
T* deleteNode(T **node, T **head, T **tail);
//head-tail�̃��X�g�̃m�[�h�����ׂď���
template<typename T>
void deleteAllNode(T **head, T **tail);
