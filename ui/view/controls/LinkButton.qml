import QtQuick 2.11
import QtQuick.Controls 1.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.0
import Beam.Wallet 1.0
import "."

Control {
    id: control

    property string linkColor: "#00f6d2"
    property string linkStyle: ["<style>a:link {color: '", linkColor, "'; text-decoration: none;}</style>"].join("")
    property string text
    property bool   semibold: false
    signal   clicked

    contentItem: SFText {
        text:           [linkStyle, "<a href='#'>", control.text, "</a>"].join("")
        textFormat:     Text.RichText
        font.pixelSize: 14
        font.styleName: semibold ? "Medium" : "Normal"
        font.weight:    semibold ? Font.Medium : Font.Normal

        MouseArea {
            id:                area
            anchors.fill:      parent
            acceptedButtons:   Qt.LeftButton
            onClicked:         control.clicked()
            hoverEnabled:      true
            onPositionChanged: area.cursorShape = Qt.PointingHandCursor;
        }
    }
}