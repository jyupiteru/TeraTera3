/**
 * @file CContainer.cpp
 * @author jupiter ()
 * @brief CContainerクラスの実装を記述したcpp
 */

#include "CContainer.h"

/**
 * @brief 各種リソースのリスト、ポインタを管理するためのリスト
 * @n <取得したいリスト名(重複不可)、<管理する際に着ける名前、汎用ポインタ>>
 */
std::unordered_map<std::string, std::shared_ptr<CListResource>> CContainer::m_listResources;

/**
     * @brief リストに安全にアクセスさせる際に使用するリスト
     * @n リストの名前、リストにアクセスできるクラスの型ID
     */
std::unordered_map<std::string, int> CContainer::m_listAccessData;

/**
     * @brief リストの型とそのIDを管理するリスト
     * @n 型情報、型ID
     * @details 引数の２個目にコンストラクタがないものを使用できずこうなった
     */
std::unordered_map<std::type_index, int> CContainer::m_listClassType;