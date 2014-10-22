/****************************************************************************
**
**  This file is a part of Fahrplan.
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License along
**  with this program.  If not, see <http://www.gnu.org/licenses/>.
**
****************************************************************************/

import bb.cascades 1.3
import Fahrplan 1.0

import "../../about.js" as About

Page {
    id: root

    property int padding: 20

    titleBar: TitleBar {
        title: qsTr("About Fahrplan") + Retranslate.onLocaleOrLanguageChanged
    }
    ScrollView {
        Container {
            topPadding: padding
            leftPadding: padding
            bottomPadding: padding
            rightPadding: padding

            Container {
                layout: StackLayout { orientation: LayoutOrientation.LeftToRight }

                ImageView {
                    property int size: 150

                    imageSource: "qrc:/data/fahrplan2_150.png"
                    minWidth: size
                    minHeight: size
                    maxWidth: size
                    maxHeight: size
                }

                Container {
                    leftPadding: padding
                    Label {
                        text: "Fahrplan"
                        topMargin: 0
                        bottomMargin: 0
                        textStyle {
                            base: SystemDefaults.TextStyles.TitleText
                        }
                    }
                    Label {
                        text: "by smurfy (maemo@smurfy.de)"
                        topMargin: 0
                        bottomMargin: 0
                    }
                    Label {
                        text: qsTr("Version: <b>%1</b>").arg(fahrplan.version)
                              + Retranslate.onLocaleOrLanguageChanged
                        textFormat: TextFormat.Html
                        topMargin: 0
                        bottomMargin: 0
                    }
                }
            }

            Label {
                multiline: true
                text: About.aboutText
                textFormat: TextFormat.Html
            }
        }
    }
}
