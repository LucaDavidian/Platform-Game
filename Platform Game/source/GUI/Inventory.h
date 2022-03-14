#ifndef INVENTORY_H
#define INVENTORY_H

#include "data structures/vector.hpp"
#include "delegates/signal.hpp"
#include <string>

class Inventory
{
public:
	struct Item
	{
		std::string mName;
	};
public:
	void AddItem(const std::string name);
	void RemoveItem(const std::string name);

	Vector<Item> const &GetItems() const { return mItems; }

	template <typename T>
	Connection AddItemInsertedListener(T &instance, void (T::*ptrToMemFun)(const std::string &name))
	{
		return mItemInsertedEvent.Bind(instance, ptrToMemFun);
	}
private:
	Vector<Item> mItems;

	SIGNAL_ONE_PARAM(ItemInsertedEvent, const std::string &);
	ItemInsertedEvent mItemInsertedEvent;
};

#endif  // INVENTORY_H