#pragma once
#include "QuadTree.hpp"
#include <iterator>
#include <vector>

namespace Base
{

  template <typename T> struct QuadItemSlot
  {
    QuadTreeElementLocation<typename std::list<QuadItemSlot<T>>::iterator> location;
    T item;
  };

  template <typename T> class QuadTreeContainer
  {
  public:
    using QuadContainerType = std::list<QuadItemSlot<T>>;

  private:
    QuadContainerType _elements;
    QuadTree<typename QuadContainerType::iterator> _root;

  public:
    QuadTreeContainer(Rectangle area, uint8_t depth = 0)
      : _root(QuadTree<typename QuadContainerType::iterator>(area, depth))
    {
    }

    void Insert(T object, Rectangle area)
    {
      QuadItemSlot<T> slot;
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

    std::vector<T> &Items()
    {
      return _elements;
    }

    void Remove(typename QuadContainerType::iterator &item)
    {
      item->location.treeLayer->erase(item->location.itemIterator);
      _elements.erase(item);
    }
  };
} // namespace Base
