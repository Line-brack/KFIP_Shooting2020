#include "DxLib.h"
#include "list.h"


/*�ȉ��Ɋ֐��̒�`������*/
template<typename T>
void pushBack(T **node, T ** head, T **tail) {
	(*node)->next = NULL;//�ǉ��m�[�h�̎���NULL

	if (*head == NULL) //��̃��X�g�̏ꍇ
		*head = *node;//�w�b�h�͒ǉ��m�[�h���w��
	if (*tail)//�����̃m�[�h�����ɂ���Ƃ�
		(*tail)->next = *node;//�O�m�[�h�̐��ǉ��m�[�h��
	(*node)->before = *tail;//�ǉ��m�[�h�̑O��O�m�[�h��
	*tail = *node;//Tail�͍Ō�̃m�[�h���w��
}
template<typename T>
T* deleteNode(T **node, T **head, T **tail) {
	T * pReturn = (*node)->next;//���̃m�[�h���w���|�C���^�̗p��
	if ((*node)->before)//�m�[�h���擪�łȂ��Ƃ�
		(*node)->before->next = pReturn;//��O�̎������̃m�[�h�ɂ���
	else
		*head = pReturn;//�w�b�h�̎w���m�[�h�����ɂ���

	if (pReturn)//p���������w���Ȃ��Ƃ�
		pReturn->before = (*node)->before;//���̃m�[�h����2�O�̃m�[�h���Ȃ�
	else//p���������w���Ƃ�
		*tail = (*node)->before;//�e�[���͑O�̃m�[�h���w��

	free(*node);//�m�[�h������
	return pReturn;//���̃|�C���^��Ԃ�

}
template<typename T>
void deleteAllNode(T **head, T **tail) {
	T *p, *pnext;//�|�C���^��2�p��
	pnext = *head;//�w�b�h�̃m�[�h���w��
	while (pnext) {//�m�[�h���Ȃ��Ȃ�܂ŌJ��Ԃ�
		p = pnext;//p��i�߂�
		pnext = p->next;//pnext��i�߂�
		free(p);//p�̃m�[�h���J��
	}
	*head = NULL;//�k��
	*tail = NULL;//�k��
}