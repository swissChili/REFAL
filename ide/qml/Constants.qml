pragma Singleton

import QtQuick 2.5
import QtQuick.Controls.Material 2.0

QtObject {
    readonly property var buttonGrey: Material.color(Material.Grey, Material.theme == Material.Dark ? Material.Shade400 : Material.Shade600)
}
