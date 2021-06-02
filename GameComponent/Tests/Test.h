#pragma once

#include "../../TeraTera3/TeraTera.h"

/**
 * @brief テスト用のクラス
 */
class Test : public ComponentBase
{
    int i = 0;
    bool flag = false;

public:
    void Update()
    {
        if (!flag)
        {
            m_gameObject->m_transform->m_color.SetValue(0, 256, 255, 1.0f);
        }
        flag = false;
    }

    void Reset()
    {
        Destroy("ui");
        //Destroy(this->m_gameObject);
        //m_gameObject->m_transform->m_worldPosition.AddValue(1, 0, 1);

        // Destroy(this->m_gameObject->FindChildObject("child"));
        // Destroy(GameObject::Find("child"));
    }

    void FirstSet()
    {
        m_gameObject->m_transform->m_color.SetValue(128, 128, 0, 1.0);
        //this->m_gameObject->AddComponent<ComCollider2D>();
        //auto obj = m_gameObject->AddChildObject("child", E_TYPEOBJECT::UI);

        // obj->GetComponent<Com2DTexture>()->LoadTexture("airplane.png");
        // obj->m_transform->m_worldPosition.SetValue(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 5 * 2, 0);
        // obj->m_transform->m_size.SetValue(472, 61, 10);
        // obj->m_transform->m_color.SetValue(255, 255, 255, 255);
    }

    void OnTriggerEnter2D(GameObject *obj)
    {
        i++;
    }

    void OnTriggerStay2D(GameObject *obj)
    {
        m_gameObject->m_transform->m_color.SetValue(256, 0, 256, 1.0);
        flag = true;
        //Destroy(obj);
    }
};