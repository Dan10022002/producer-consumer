// Copyright 2021 Dan10022002 dan10022002@mail.ru

#include <parser.h>
#include <gumbo.h>
#include <regex>

std::vector<std::string> _formats = {
    "png", "apng", "jpg", "jpeg", "pjpeg", "gif", "webp", "bmp",
    "ico", "cur", "svg", "tif", "tiff", "pjp", "avif", "jfif"};

bool check_image(std::string& _value) {
  size_t _pos = _value.find_last_of('.');
  if (_pos == std::string::npos) {
    return false;
  }
  std::string _result = _value.substr(_pos + 1, _value.size() - 1);
  for (std::string& i : _formats) {
    if (_result == i) {
      return true;
    }
  }
  return false;
}

static void search_for_links(GumboNode* node, parameters& tmp_page) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href = nullptr;
  if (node->v.element.tag == GUMBO_TAG_A) {
    href = gumbo_get_attribute(&node->v.element.attributes, "href");
  }
  if (node->v.element.tag == GUMBO_TAG_IMG) {
    href = gumbo_get_attribute(&node->v.element.attributes, "src");
  }

  if (href) {
    std::regex _regex("^http[s]?://.*)|(/.*))");
    std::string _value = href->value;
    if (!regex_match(_value.begin(), _value.end(), _regex)) {
      return;
    } else if (_value[0] == '/') {
      _value = tmp_page._protocol + tmp_page._host + href->value;
    }

    if (check_image(_value)) {
      write_queue.push(std::move(_value));
    } else {
      if (tmp_page._depth == 1) {
        return;
      }
      URL tmp_URL{_value, tmp_page._depth - 1};
      URL_queue.push(std::move(tmp_URL));
    }
  }
  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]), tmp_page);
  }
}

void parser::parse() {
  if (!page_queue.empty()) {
    parameters tmp_page = page_queue.front();
    GumboOutput* output = gumbo_parse(tmp_page._page.c_str());
    search_for_links(output->root, tmp_page);
    gumbo_destroy_output(&kGumboDefaultOptions, output);
    page_queue.pop();
  }
}
