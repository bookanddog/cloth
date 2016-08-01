#ifndef KEY
#define KEY

#include <string.h>
//定义键盘类
class Keys
{
public:
	Keys() { Clear(); }

	//清空所有按键信息
	void Clear() { /*ZeroMemory(&m_keyDown, sizeof(m_keyDown));*/ memset(&m_keyDown, 0, MAX_KEYS * sizeof(bool));}

	//判断某个键是否按下
	bool IsPressed(unsigned int key)    { return (key < MAX_KEYS) ? (m_keyDown[key] == true) : false; }

	//设置莫个键按下
	void SetPressed(unsigned int key)   { if (key < MAX_KEYS) m_keyDown[key] = true; }

	//设置某个键被释放
	void SetReleased(unsigned int key)  { if (key < MAX_KEYS) m_keyDown[key] = false; }

private:
	static const unsigned int MAX_KEYS = 256;
	bool m_keyDown[MAX_KEYS];                    //保存256个键的状态
};
#endif // KEY

