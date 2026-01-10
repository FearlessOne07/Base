#pragma once
#include "Circle.hpp"
#include "base/rendering/Quad.hpp"
#include <array>
#include <cstdint>
#include <iterator>
#include <list>
#include <memory>
#include <utility>
#include <variant>

namespace Base
{

  using ItemAreaType = std::variant<Rectangle, Circle>;

  template <typename T> struct QuadTreeElementLocation
  {
    typename std::list<std::pair<ItemAreaType, T>>::iterator itemIterator;
    std::list<std::pair<ItemAreaType, T>> *treeLayer;
  };

  template <typename T> class QuadTree
  {
  private:
    constexpr static uint8_t MAX_DEPTH = 12;
    constexpr static size_t MAX_ITEMS_PER_NODE = 100;

    Rectangle _area;
    uint8_t _depth = 0;
    bool _divided = false;

    std::array<Rectangle, 4> _childAreas;
    std::array<std::shared_ptr<QuadTree<T>>, 4> _children;
    std::list<std::pair<ItemAreaType, T>> _layerItems;

  private:
    void _subdivide()
    {
      if (_divided)
        return;

      Vector2 childSize = {_area.GetSize().x / 2.0f, _area.GetSize().y / 2.0f};

      _childAreas = {
        Rectangle{{
                    _area.GetPosition().x,
                    _area.GetPosition().y,
                  },
                  {childSize.x, childSize.y}}, // NW
        Rectangle{{
                    _area.GetPosition().x + childSize.x,
                    _area.GetPosition().y,
                  },
                  {childSize.x, childSize.y}}, // NE
        Rectangle{{
                    _area.GetPosition().x,
                    _area.GetPosition().y + childSize.y,
                  },
                  {childSize.x, childSize.y}}, // SW
        Rectangle{{
                    _area.GetPosition().x + childSize.x,
                    _area.GetPosition().y + childSize.y,
                  },
                  {childSize.x, childSize.y}}, // SE
      };

      for (int i = 0; i < 4; i++)
      {
        _children[i] = std::make_shared<QuadTree<T>>(_childAreas[i], _depth + 1);
      }

      _divided = true;
    }

    bool _fitsEntirelyRect(Rectangle rect, Rectangle container)
    {
      return rect.GetPosition().x >= container.GetPosition().x && rect.GetPosition().y >= container.GetPosition().y &&
             rect.GetPosition().x + rect.GetSize().x <= container.GetPosition().x + container.GetSize().x &&
             rect.GetPosition().y + rect.GetPosition().y <= container.GetPosition().y + container.GetSize().y;
    }

    bool _fitsEntirelyCircle(Circle circle, Rectangle container)
    {
      return (circle.position.x - circle.radius) >= container.GetPosition().x &&
             (circle.position.y - circle.radius) >= container.GetPosition().y &&
             (circle.position.x + circle.radius) <= (container.GetPosition().x + container.GetSize().x) &&
             (circle.position.y + circle.radius) <= (container.GetPosition().y + container.GetSize().y);
    }

    void _search(ItemAreaType searchArea, std::list<T> &results)
    {
      // Check items at this level
      for (auto &[area, item] : _layerItems)
      {
        if (std::holds_alternative<Rectangle>(area) && std::holds_alternative<Rectangle>(searchArea))
        {
          if (CheckCollisionRecs(std::get<Rectangle>(area), std::get<Rectangle>(searchArea)))
          {
            results.push_back(item);
          }
        }
        if (std::holds_alternative<Circle>(area) && std::holds_alternative<Circle>(searchArea))
        {
          Circle circle1 = std::get<Circle>(area);
          Circle circle2 = std::get<Circle>(searchArea);
          if (CheckCollisionCircles(circle1.position, circle1.radius, circle2.position, circle2.radius))
          {
            results.push_back(item);
          }
        }
        else if (std::holds_alternative<Circle>(area) && std::holds_alternative<Rectangle>(searchArea))
        {
          Circle circle = std::get<Circle>(area);
          CheckCollisionCircleRec(circle.position, circle.radius, std::get<Rectangle>(searchArea));
        }
        else if (std::holds_alternative<Circle>(searchArea) && std::holds_alternative<Rectangle>(area))
        {
          Circle circle = std::get<Circle>(searchArea);
          if (CheckCollisionCircleRec(circle.position, circle.radius, std::get<Rectangle>(area)))
          {
            results.push_back(item);
          }
        }
      }

      // Check children if subdivided
      if (_divided)
      {
        for (int i = 0; i < 4; i++)
        {
          if (std::holds_alternative<Rectangle>(searchArea))
          {
            if (CheckCollisionRecs(std::get<Rectangle>(searchArea), _childAreas[i]))
            {
              _children[i]->_search(searchArea, results);
            }
          }
          else if (std::holds_alternative<Circle>(searchArea))
          {
            Circle circle = std::get<Circle>(searchArea);
            if (CheckCollisionCircleRec(circle.position, circle.radius, _childAreas[i]))
            {
              _children[i]->_search(searchArea, results);
            }
          }
        }
      }
    }

  public:
    QuadTree(Rectangle area, uint8_t depth = 0) : _area(area), _depth(depth)
    {
    }

    QuadTreeElementLocation<T> Insert(T object, ItemAreaType area)
    {
      // If we haven't subdivided and we're not at max depth and we have too many
      // items
      if (!_divided && _depth < MAX_DEPTH && _layerItems.size() >= MAX_ITEMS_PER_NODE)
      {
        _subdivide();
      }

      // Try to insert into children if subdivided
      if (_divided)
      {
        bool insertedIntoChild = false;

        // Find the best child to insert into (the one that completely contains
        // the area)
        for (int i = 0; i < 4; i++)
        {
          if (std::holds_alternative<Rectangle>(area))
          {
            if (_fitsEntirelyRect(std::get<Rectangle>(area), _childAreas[i]))
            {
              insertedIntoChild = true;
              return _children[i]->Insert(object, area);
            }
          }
          else if (std::holds_alternative<Circle>(area))
          {
            if (_fitsEntirelyCircle(std::get<Circle>(area), _childAreas[i]))
            {
              insertedIntoChild = true;
              return _children[i]->Insert(object, area);
            }
          }
        }

        // If no child completely contains it, store at this level
        if (!insertedIntoChild)
        {
          _layerItems.push_back({area, object});
          auto it = std::prev(_layerItems.end());
          return {it, &_layerItems};
        }
      }
      else
      {
        // Not subdivided, store at this level
        _layerItems.push_back({area, object});
        auto it = std::prev(_layerItems.end());
        return {it, &_layerItems};
      }

      return {};
    }

    void Clear()
    {
      _layerItems.clear();
      _divided = false;
      for (int i = 0; i < 4; i++)
      {
        if (_children[i])
        {
          _children[i]->Clear();
          _children[i].reset();
        }
      }
    }

    std::list<T> Search(ItemAreaType area)
    {
      std::list<T> results;
      _search(area, results);
      return results;
    }

    // Debug function to visualize the tree structure
    void DrawBounds(Color color = {255, 0, 0, 0})
    {
      DrawRectangleLinesEx(_area, 1.0f, color);

      if (_divided)
      {
        for (int i = 0; i < 4; i++)
        {
          _children[i]->DrawBounds(color);
        }
      }
    }
  };
} // namespace Base
