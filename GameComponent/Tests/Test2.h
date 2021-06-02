#pragma once

#include "../../TeraTera3/TeraTera.h"

/**
 * @brief テスト用のクラス
 */
class Test2 : public ComponentBase
{
    int i = 0;

public:
    void Reset()
    {
        //Destroy(this);
        //Destroy(this->m_gameObject);
        //m_gameObject->m_transform->m_worldPosition.AddValue(1, 0, 1);

        // Destroy(this->m_gameObject->FindChildObject("child"));
        // Destroy(GameObject::Find("child"));
    }

    void MakeChildObject(int num, int childnum)
    {
        int i = 0;
        while (i < num)
        {
            std::string name = m_gameObject->m_objectName + " " + std::to_string(num)+" " +std::to_string(i);
            auto obj = m_gameObject->AddChildObject(name, E_TYPE_OBJECT::MODEL3D);
            obj->m_typeObject = E_TYPE_OBJECT::MODEL3D;
            obj->GetComponent<Com3DModelAssimp>()->LoadModelData("Hal/hal.pmx", "HAL/");
            obj->m_transform->m_size.SetValue(50.0f, 50.0f, 50.0f);
            //obj->AddComponent<ComBoxCollider3D>();
            obj->AddComponent<Test2>()->MakeMoreChildObject(childnum);
            //obj->RemoveComponent<ComTransform>();
            /*obj->RemoveComponent<Test2>();
            obj->GetComponent<Test2>()->m_enable.SetValue(false);
            obj->GetComponent<ComTransform>()->m_enable.SetValue(false);*/
            //obj->GetComponent<ComTransform>()->m_worldPosition.SetValue(0, 0, i * 10);
            i++;
        }
    }

    void MakeMoreChildObject(int num)
    {
        if (num -= 1; num > 0)
        {
            std::string name = m_gameObject->m_objectName + " more : " + std::to_string(num);
            auto obj = m_gameObject->AddChildObject(name, E_TYPE_OBJECT::NONE);
            obj->AddComponent<Test2>()->MakeMoreChildObject(num);
           // obj->GetComponent<ComTransform>()->m_worldPosition.SetValue(0, 0, -num * 10);
        }
    }

    void OnTriggerEnter2D(GameObject *obj)
    {
        i++;
    }

    void OnTriggerEnter3D(GameObject *obj)
    {
        i++;
        m_gameObject->m_transform->m_vector.SetValue(0, 0, 0);
    }

    void OnTriggerStay3D(GameObject *obj)
    {
        i++;
        m_gameObject->m_transform->m_vector.SetValue(0, 0, 0);
    }
};