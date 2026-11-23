//====================================================================================
//
// Belenus WebApp alkalmazas (c) Bikfalvi Tamas
//
//====================================================================================

#pragma once

#include <QString>

#include <functional>
#include <utility>

// A felületi üzenet a fordítási műveletet és annak paramétereit őrzi.
// A szöveg mindig a megjelenítéskor aktív fordítóból készül el.
class UiMessage {
public:
    UiMessage() = default;
    explicit UiMessage(std::function<QString()> render) : render(std::move(render)) {}

    bool isEmpty() const { return !render; }
    QString text() const { return render ? render() : QString(); }

private:
    std::function<QString()> render;
};
