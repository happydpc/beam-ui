import QtQuick 2.11
import QtQuick.Controls 1.2
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.2
import QtQuick.Layouts 1.0
import QtGraphicalEffects 1.0
import Beam.Wallet 1.0
import "."

SettingsFoldable {
    property var viewModel

    title: viewModel.localNodeRun ?
       //% "Beam Integrated Node"
       qsTrId("settings-integrated-node-title") :
       //% "Beam Remote Node"
       qsTrId("settings-remote-node-title")

    content: ColumnLayout {
        spacing: 20
        RowLayout {
            SFText {
                //: settings tab, node section, run node label
                //% "Integrated node"
                text: qsTrId("settings-local-node-run-checkbox")
                color: viewModel.localNodeRun ? Style.active : Style.content_secondary
                font.pixelSize: 14
            }
            Item {width: 3}
            CustomSwitch {
                id: localNodeRun
                checked: !viewModel.localNodeRun
                alwaysGreen: true
                spacing: 0
                Binding {
                    target: viewModel
                    property: "localNodeRun"
                    value: !localNodeRun.checked
                }
            }
            Item {width: 3}
            SFText {
                //% "Remote node"
                text: qsTrId("settings-run-remote-node")
                color: viewModel.localNodeRun ? Style.content_secondary : Style.active
                font.pixelSize: 14
            }
            Item {
                Layout.fillWidth: true
            }
        }

        //
        // Remote node settings
        //
        GridLayout {
            Layout.fillWidth: true
            visible: !viewModel.localNodeRun
            columnSpacing: 20
            rowSpacing: 15
            columns: 2

            SFText {
                //: settings tab, node section, address label
                //% "Remote node address"
                text: qsTrId("settings-remote-node-address")
                color: Style.content_secondary
                Layout.fillWidth: true
                font.pixelSize: 14
                wrapMode: Text.NoWrap
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                spacing: 0

                SFTextInput {
                    id: nodeAddress
                    Layout.preferredWidth: 100
                    Layout.alignment: Qt.AlignRight
                    topPadding: 0
                    focus: true
                    activeFocusOnTab: true
                    font.pixelSize: 14
                    color:  (!viewModel.isValidNodeAddress || !nodeAddress.acceptableInput) ? Style.validator_error : Style.content_main
                    backgroundColor:  (!viewModel.isValidNodeAddress || !nodeAddress.acceptableInput) ? Style.validator_error : Style.content_main
                    validator: RegExpValidator { regExp: /^(\s|\x180E)*((([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])|([\w.-]+(?:\.[\w\.-]+)+))(\s|\x180E)*$/ }
                    text: viewModel.nodeAddress
                    Binding {
                        target: viewModel
                        property: "nodeAddress"
                        value: nodeAddress.text.trim()
                    }
                }
                Item {
                    id: nodeAddressError
                    Layout.preferredWidth: 100
                    Layout.alignment: Qt.AlignRight
                    SFText {
                        color:          Style.validator_error
                        font.pixelSize: 12
                        font.italic:    true
                        text:           qsTrId("general-invalid-address")
                        visible:        (!viewModel.isValidNodeAddress || !nodeAddress.acceptableInput)
                    }
                }
            }

            // remote port
            SFText {
                text: qsTrId("settings-local-node-port")
                color: Style.content_secondary
                Layout.fillWidth: true
                font.pixelSize: 14
                wrapMode: Text.NoWrap
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                spacing: 0

                SFTextInput {
                    id: remoteNodePort
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 100
                    activeFocusOnTab: true
                    font.pixelSize: 14
                    color: !remoteNodePort.acceptableInput ? Style.validator_error : Style.content_main
                    text: viewModel.remoteNodePort
                    validator: RegExpValidator {regExp: /^([1-9]|[1-5]?[0-9]{2,4}|6[1-4][0-9]{3}|65[1-4][0-9]{2}|655[1-2][0-9]|6553[1-5])$/g}
                    backgroundColor: !remoteNodePort.acceptableInput ? Style.validator_error : Style.content_main
                    Binding {
                        target: viewModel
                        property: "remoteNodePort"
                        value: remoteNodePort.text
                    }
                }
                Item {
                    id: nodePortError
                    Layout.preferredWidth: 100
                    Layout.alignment: Qt.AlignRight
                    SFText {
                        color:          Style.validator_error
                        font.pixelSize: 12
                        font.italic:    true
                        //: settings tab, node section, port error label
                        //% "Port is mandatory"
                        text:           qsTrId("general-invalid-port")
                        visible:        !remoteNodePort.acceptableInput
                    }
                }
            }
        }

        //
        // Integrated node settings
        //
        GridLayout {
            Layout.fillWidth: true
            visible: viewModel.localNodeRun
            columnSpacing: 20
            rowSpacing: 15
            columns: 2

            SFText {
                //: settings tab, node section, port label
                //% "Port"
                text: qsTrId("settings-local-node-port")
                color: Style.content_secondary
                font.pixelSize: 14
                wrapMode: Text.NoWrap
                Layout.fillWidth: true
            }

            ColumnLayout {
                Layout.alignment: Qt.AlignTop
                spacing: 0

                SFTextInput {
                    id: localNodePort
                    Layout.alignment: Qt.AlignRight
                    Layout.preferredWidth: 100
                    topPadding: 0
                    activeFocusOnTab: true
                    font.pixelSize: 14
                    color: !localNodePort.acceptableInput ? Style.validator_error : Style.content_main
                    text: viewModel.localNodePort
                    validator: RegExpValidator {regExp: /^([1-9]|[1-5]?[0-9]{2,4}|6[1-4][0-9]{3}|65[1-4][0-9]{2}|655[1-2][0-9]|6553[1-5])$/g}
                    backgroundColor: !localNodePort.acceptableInput ? Style.validator_error : Style.content_main
                    Binding {
                        target: viewModel
                        property: "localNodePort"
                        value: localNodePort.text
                    }
                }
                Item {
                    id: localNodePortError
                    Layout.preferredWidth: 100
                    Layout.alignment: Qt.AlignRight
                    SFText {
                        color:          Style.validator_error
                        font.pixelSize: 12
                        font.italic:    true
                        text:           qsTrId("general-invalid-port")
                        visible:        !localNodePort.acceptableInput
                    }
                }
            }
        }

        SFText {
            //% "Peers"
            text: qsTrId("settings-peers-title")
            color: Style.content_main
            font.pixelSize: 16
            font.styleName: "Bold"; font.weight: Font.Bold
            visible: viewModel.localNodeRun
            Layout.topMargin: 10
        }

        RowLayout {
            spacing: 0
            visible: viewModel.localNodeRun

            SFTextInput {
                Layout.preferredWidth: nodeBlock.width * 0.7
                id: newLocalNodePeer
                activeFocusOnTab: true
                font.pixelSize: 14
                color: Style.content_main
                validator: RegExpValidator { regExp: /^(\s|\x180E)*((([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])\.){3}([0-9]|[1-9][0-9]|1[0-9]{2}|2[0-4][0-9]|25[0-5])|([\w.-]+(?:\.[\w\.-]+)+))(:([1-9]|[1-9][0-9]{1,3}|[1-5][0-9]{4}|6[0-4][0-9]{3}|65[0-4][0-9]{2}|655[0-2][0-9]|6553[0-5]))?(\s|\x180E)*$/ }
            }

            Item {
                Layout.fillWidth: true
            }

            Image {
                Layout.alignment: Qt.AlignRight
                Layout.preferredHeight: 16
                Layout.preferredWidth: 16
                source: "qrc:/assets/icon-add-green.svg"
                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.LeftButton
                    cursorShape: newLocalNodePeer.acceptableInput ? Qt.PointingHandCursor : Qt.ArrowCursor
                    onClicked: {
                        if (newLocalNodePeer.acceptableInput) {
                            viewModel.addLocalNodePeer(newLocalNodePeer.text.trim());
                            newLocalNodePeer.clear();
                        }
                    }
                }
                Colorize {
                    anchors.fill: parent
                    source: parent
                    saturation: -0.5
                    visible: !newLocalNodePeer.acceptableInput
                }
            }
            Item {
                width: 8
            }
        }

        ListView {
            visible: viewModel.localNodeRun
            Layout.fillWidth: true
            Layout.preferredHeight: (viewModel.localNodePeers.length || 1 ) * 36
            model: viewModel.localNodePeers
            id: peersList
            
            delegate: RowLayout {
                width: parent.width
                height: 36
                SFText {
                    Layout.alignment: Qt.AlignVCenter
                    text: modelData
                    font.pixelSize: 14
                    color: Style.content_main
                    height: 16
                    elide: Text.ElideRight
                }
                CustomToolButton {
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                    padding: 0
                    spacing: 0
                    icon.source: "qrc:/assets/icon-delete.svg"
                    enabled: !localNodeRun.checked
                    onClicked: viewModel.deleteLocalNodePeer(index)
                }
            }
        }

        //
        // Buttons
        //
        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignHCenter
            Layout.topMargin: 5

            CustomButton {
                Layout.preferredHeight: 38
                leftPadding:  25
                rightPadding: 25
                spacing: 12
                //% "Cancel"
                text: qsTrId("general-cancel")
                icon.source: "qrc:/assets/icon-cancel-white.svg"
                enabled: viewModel.isNodeChanged
                onClicked: viewModel.undoChanges()
            }

            Item {
                width: 20
            }

            PrimaryButton {
                Layout.preferredHeight: 38
                leftPadding:  25
                rightPadding: 25
                spacing: 12
                //: settings tab, node section, apply button
                //% "Apply changes"
                text: qsTrId("settings-apply")
                icon.source: "qrc:/assets/icon-done.svg"
                enabled: {
                    if (!viewModel.isNodeChanged) return false;
                    if (!localNodeRun.checked) return viewModel.localNodePeers.length > 0 && localNodePort.acceptableInput
                    return viewModel.isValidNodeAddress && nodeAddress.acceptableInput && remoteNodePort.acceptableInput
                }
                onClicked: viewModel.applyChanges()
            }
        }
    }
}
