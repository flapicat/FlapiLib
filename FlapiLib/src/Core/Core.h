#pragma once
#include <memory>

template<typename T>
using Ref = std::shared_ptr<T>;

template<typename T>
std::shared_ptr<T> CreateRef()
{
	return std::make_shared<T>();
}

template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T>
std::unique_ptr<T> CreateScope()
{
	return std::make_unique<T>();
}