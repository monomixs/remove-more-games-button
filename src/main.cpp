#include <Geode/modify/MenuLayer.hpp>
#include <algorithm>
#include <cctype>
#include <string>

using namespace geode::prelude;

// Walks the node tree looking for the first node whose string ID
// looks like it belongs to the "More Games" button.
CCNode* findMoreGamesButton(CCNode* node) {
    auto children = node->getChildren();
    if (!children) return nullptr;

    int count = children->count();
    for (int i = 0; i < count; i++) {
        auto child = typeinfo_cast<CCNode*>(children->objectAtIndex(i));
        if (!child) continue;

        auto id = std::string(child->getID());
        std::transform(id.begin(), id.end(), id.begin(), [](unsigned char c) {
            return static_cast<char>(std::tolower(c));
        });
        if (id.find("more-games") != std::string::npos ||
            id.find("moregames") != std::string::npos) {
            return child;
        }

        if (auto found = findMoreGamesButton(child)) {
            return found;
        }
    }
    return nullptr;
}

class $modify(MenuLayer) {
    bool init() {
        if (!MenuLayer::init()) return false;

        if (auto btn = findMoreGamesButton(this)) {
            auto menu = btn->getParent();
            btn->removeFromParent();
            if (menu) menu->updateLayout(); // closes the gap left behind
        } else {
            log::warn("REMOVE MORE GAMES: couldn't locate the button by ID.");
        }

        return true;
    }
};