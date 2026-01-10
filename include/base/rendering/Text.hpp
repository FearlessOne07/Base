#pragma once
#include "font/Font.hpp"
#include "renderer/renderable/Renderable.hpp"
#include <memory>

namespace Base
{

class Text : public Renderable
{
private:
  std::shared_ptr<Font> _font;
  std::string _value;

public:
  Text(const std::shared_ptr<Font> &font, const std::string &value);
  Text(const std::shared_ptr<Font> &font, const std::string &value, const Material &material);
  const std::shared_ptr<Font> &GetFont() const;
  const std::string &GetValue() const;
};

} // namespace Base
