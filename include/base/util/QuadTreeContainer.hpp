#pragma once
#include "QuadTree.hpp"
#include <iterator>

namespace Base
{

  template <typename T> struct QuadTreeItem
  {
    QuadTreeElementLocation<typename std::list<QuadTreeItem<T>>::iterator> location;
    T item;
  };

  template <typename T> class QuadTreeContainer
  {
  public:
    using QuadContainerType = std::list<QuadTreeItem<T>>;

  private:
    QuadContainerType _elements;
    QuadTree<typename QuadContainerType::iterator> _root;

  public:
    QuadTreeContainer(Rectangle area, uint8_t depth = 0)
      : _root(QuadTree<typename QuadContainerType::iterator>(area, depth))
    {
    }

    void Insert(T object, ItemAreaType area)
    {
      QuadTreeItem<T> slot;
      slot.item = object;
      _elements.push_back(slot);
      _elements.back().location = _root.Insert(std::prev(_elements.end()), area);
    }

    void Clear()
    {
      _root.Clear();
    }

    std::list<typename QuadContainerType::iterator> Search(ItemAreaType area)
    {
      return _root.Search(area);
    }

    void Remove(const typename QuadContainerType::iterator &item)
    {
      item->location.treeLayer->erase(item->location.itemIterator);
      _elements.erase(item);
    }

    void UpdateItem(typename QuadContainerType::iterator item, ItemAreaType area)
    {
      item->location.treeLayer->erase(item->location.itemIterator);
      item->location = _root.Insert(item, area);
    }

    typename QuadContainerType::iterator begin()
    {
      return _elements.begin();
    }
    typename QuadContainerType::iterator end()
    {
      return _elements.end();
    }

    typename QuadContainerType::const_iterator cbegin()
    {
      return _elements.cbegin();
    }

    typename QuadContainerType::const_iterator cend()
    {
      return _elements.cend();
    }

    size_t Size() const
    {
      return _elements.size();
    }
  };
} // namespace Base
