/**
 * @file 		CVector.h
 * @author 		jupiter
 * @brief 		CVectorが記載されたヘッダー
 */

#include <tuple>

#pragma once

/**
 * @brief 変数を１つ保持するクラス
 * @tparam type 保持されたい型の種類
 */
template <typename type>
class CVector
{
	type x;

public:
	CVector() { SetValue(static_cast<type>(0)); };

	CVector(type x) = delete;

	~CVector(){};

	//!@brief		値をセットする
	//!@param	x	xに入れたいものを入れる
	void SetValue(type x)
	{
		this->x = x;
	}

	/*!
	@brief 値を足す
	@param	x xに足したい値を入れる
	*/
	void AddValue(type x)
	{
		this->x += x;
	}

	//!@brief このクラスの保持している値を取得する
	//!@return このクラスの保持している値
	[[nodiscard]] type GetValue() const
	{
		return x;
	}

	//オペレーター
	void operator+=(CVector vec)
	{
		this->x += vec.x;
	}
	void operator-=(CVector vec)
	{
		this->x -= vec.x;
	}
	void operator*=(CVector vec)
	{
		this->x *= vec.x;
	}
	void operator/=(CVector vec)
	{
		this->x /= vec.x;
	}

	auto operator+(CVector vec) const
	{
		return CVector(this->x + vec.x);
	}
	auto operator-(CVector vec) const
	{
		return CVector(this->x - vec.x);
	}
	auto operator*(CVector vec) const
	{
		return CVector(this->x * vec.x);
	}
	auto operator/(CVector vec) const
	{
		return CVector(this->x / vec.x);
	}
};

/**
 * @brief 2つの値を保持するクラス
 * @tparam type 保持させたい型情報
 */
template <typename type>
class CVector2
{
	type x, y;

public:
	CVector2() : x(static_cast<type>(0)), y(static_cast<type>(0)){};

	//演算子のオーバーロードで型推論から自動生成されカプセル化が死ぬので削除
	//CVector2(type _x, type _y) : x(_x), y(_y){};

	~CVector2(){};

	//!@brief		値をセットする1
	//!@param	x	xに入れたいものを入れる
	//!@param	y	yに入れたいものを入れる
	void SetValue(type x, type y)
	{
		this->x = x;
		this->y = y;
	}

	/**
     * @brief Set the Value object
     * @param vec pair型のもの
     */
	void SetValue(std::pair<type, type> vec)
	{
		SetValue(vec.first, vec.second);
	}

	/*!
	@brief			値を足す1
	@param	x		xに足したい値を入れる
	@param	y		yに足したい値を入れる
	*/
	void AddValue(type x, type y)
	{
		this->x += x;
		this->y += y;
	}

	/**
	 * @brief 値を足す2
	 * @param vec pair型のもの
	 */
	void AddValue(std::pair<type, type> vec)
	{
		AddValue(vec.first, vec.second);
	}

	/**
	 * @brief このクラスの保持している値を取得する
	 * @n  auto[,](構造体束縛)やstd::tie(,)等で受け取る
	 * @details 参照で渡されるが実際は違うので変更しても反映されません
	 * @return std::pair<type, type> このクラスの保持している値
	 */
	[[nodiscard]] std::pair<type, type> GetValue() const
	{
		return {x, y};
	}

	//オペレーター
	void operator+=(CVector2 vec)
	{
		this->x += vec.x;
		this->y += vec.y;
	}
	void operator-=(CVector2 vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
	}
	void operator*=(CVector2 vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
	}
	void operator/=(CVector2 vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
	}

	auto operator+(CVector2 vec) const
	{
		return CVector2(this->x + vec.x, this->y + vec.y);
	}
	auto operator-(CVector2 vec) const
	{
		return CVector2(this->x - vec.x, this->y - vec.y);
	}
	auto operator*(CVector2 vec) const
	{
		return CVector2(this->x * vec.x, this->y * vec.y);
	}
	auto operator/(CVector2 vec) const
	{
		return CVector2(this->x / vec.x, this->y / vec.y);
	}
};

/**
 * @brief 3つの値を保持するクラス
 * @tparam type 保持させたい型情報
 */
template <typename type>
class CVector3
{
	type x, y, z;

public:
	CVector3() : x(static_cast<type>(0)), y(static_cast<type>(0)), z(static_cast<type>(0)){};

	//演算子のオーバーロードで型推論から自動生成されカプセル化が死ぬので削除
	// CVector3(type _x, type _y, type _z) : x(_x), y(_y), z(_z){};

	~CVector3(){};

	//!@brief		値をセットする1
	//!@param	x	xに入れたいものを入れる
	//!@param	y	yに入れたいものを入れる
	//!@param	z	zに入れたいものを入れる
	void SetValue(type x, type y, type z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	/**
	 * @brief 値をセットする2
	 * @param vec セットしたいtuple<type,type,type>型をいれる
	 */
	void SetValue(std::tuple<type, type, type> vec)
	{
		SetValue(std::get<0>(vec), std::get<1>(vec), std::get<2>(vec));
	}

	/*!
	@brief			値を足す1
	@param	x		xに足したい値を入れる
	@param	y		yに足したい値を入れる
	@param	z		zに足したい値を入れる
	*/
	void AddValue(type x, type y, type z)
	{
		this->x += x;
		this->y += y;
		this->z += z;
	}

	/**
	 * @brief 当たりを足す２
	 * @param vec 足したいstd::tuple<type,type,type>型
	 */
	void AddValue(std::tuple<type, type, type> vec)
	{
		AddValue(std::get<0>(vec), std::get<1>(vec), std::get<2>(vec));
	}

	/**
	 * @brief このクラスの保持している値を取得する
	 * @n  auto[,,](構造体束縛)やstd::tie(,,)等で受け取る
	 * @details 参照で渡されるが実際は違うので変更しても反映されません
	 * @return std::tuple<type, type, type> このクラスの保持している値
	 */
	[[nodiscard]] std::tuple<type, type, type> GetValue() const
	{
		return {x, y, z};
	}

	//オペレーター
	void operator+=(CVector3 vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
	}

	void operator-=(CVector3 vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
	}

	void operator*=(CVector3 vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
	}

	void operator/=(CVector3 vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;
	}

	auto operator+(CVector3 vec) const
	{
		return CVector3(this->x + vec.x, this->y + vec.y, this->z + vec.z);
	}
	auto operator-(CVector3 vec) const
	{
		return CVector3(this->x - vec.x, this->y - vec.y, this->z - vec.z);
	}
	auto operator*(CVector3 vec) const
	{
		return CVector3(this->x * vec.x, this->y * vec.y, this->z * vec.z);
	}
	auto operator/(CVector3 vec) const
	{
		return CVector3(this->x / vec.x, this->y / vec.y, this->z / vec.z);
	}
};

/**
 * @brief 4つの値を保持するクラス
 * @tparam type 保持させたい型情報
 */
template <typename type>
class CVector4
{
	type x, y, z, w;

public:
	CVector4() : x(static_cast<type>(0)), y(static_cast<type>(0)), z(static_cast<type>(0)), w(static_cast<type>(0)){};

	//演算子のオーバーロードで型推論から自動生成されカプセル化が死ぬので削除
	// CVector4(type _x, type _y, type _z, type _w) : x(_x), y(_y), z(_z), w(_w){};

	~CVector4(){};

	/**
	 * @brief 値をセットする１
	 * @param x xにセットしたいものを入れる
	 * @param y yにセットしたいものを入れる
	 * @param z zにセットしたいものを入れる
	 * @param w wにセットしたいものを入れる
	 */
	void SetValue(type x, type y, type z, type w)
	{
		this->x = x;
		this->y = y;
		this->z = z;
		this->w = w;
	}

	/**
	 * @brief 値をセットする2
	 * @param vec セットしたいstd::tuple<type,type,type,type>型の値
	 */
	void SetValue(std::tuple<type, type, type, type> vec)
	{
		SetValue(std::get<0>(vec), std::get<1>(vec), std::get<2>(vec), std::get<3>(vec));
	}

	/**
	 * @brief 値を足す1
	 * @param x xに足したい値を入れる
	 * @param y yに足したい値を入れる
	 * @param z zに足したい値を入れる
	 * @param w wに足したい値を入れる
	 */
	void AddValue(type x, type y, type z, type w)
	{
		this->x += x;
		this->y += y;
		this->z += z;
		this->w += w;
	}

	/**
	 * @brief 値を足す処理2
	 * @param vec 足したいstd::tuple<type,type,type,type>の値
	 */
	void AddValue(std::tuple<type, type, type, type> vec)
	{
		AddValue(std::get<0>(vec), std::get<1>(vec), std::get<2>(vec), std::get<3>(vec));
	}

	/**
	 * @brief このクラスの保持している値を取得する
	 * @n  auto[,,](構造体束縛)やstd::tie(,,)等で受け取る
	 * @details 参照で渡されるが実際は違うので変更しても反映されません
	 * @return std::tuple<type, type, type,type> このクラスの保持している値
	 */
	[[nodiscard]] std::tuple<type, type, type, type> GetValue()
	{
		return {x, y, z, w};
	}

	//オペレーター
	void operator+=(CVector4 vec)
	{
		this->x += vec.x;
		this->y += vec.y;
		this->z += vec.z;
		this->w += vec.w;
	}
	void operator-=(CVector4 vec)
	{
		this->x -= vec.x;
		this->y -= vec.y;
		this->z -= vec.z;
		this->w -= vec.w;
	}
	void operator*=(CVector4 vec)
	{
		this->x *= vec.x;
		this->y *= vec.y;
		this->z *= vec.z;
		this->w *= vec.w;
	}
	void operator/=(CVector4 vec)
	{
		this->x /= vec.x;
		this->y /= vec.y;
		this->z /= vec.z;
		this->w /= vec.w;
	}

	auto operator+(CVector4 vec)
	{
		return CVector4(this->x + vec.x, this->y + vec.y, this->z + vec.z, this->w + vec.w);
	}
	auto operator-(CVector4 vec)
	{
		return CVector4(this->x - vec.x, this->y - vec.y, this->z - vec.z, this->w - vec.w);
	}
	auto operator*(CVector4 vec)
	{
		return CVector4(this->x * vec.x, this->y * vec.y, this->z * vec.z, this->w * vec.w);
	}
	auto operator/(CVector4 vec)
	{
		return CVector4(this->x / vec.x, this->y / vec.y, this->z / vec.z, this->w / vec.w);
	}
};