# QWinHelp project file.
#
# Copyright (C) 2010 rel-eng
#
# This file is part of QWinHelp.
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = app
TARGET = QWinHelp
QT += core \
    gui \
    network \
    webkit
HEADERS += HelpReply.h \
    HelpAccessManager.h \
    HelpView.h \
    HelpIndexModel.h \
    WinHelp/TopicLink.h \
    WinHelp/TopicHeader.h \
    WinHelp/TopicRawDescriptor.h \
    WinHelp/DisplayableText.h \
    WinHelp/DisplayableTable.h \
    WinHelp/DisplayableTextNew.h \
    WinHelp/FormattingCommands.h \
    WinHelp/DisplayableTextOld.h \
    ThreadedWinHelpFileLoader.h \
    WinHelp/TopicHeaderNew.h \
    WinHelp/TopicHeaderOld.h \
    WinHelpApplication.h \
    WinHelp/TopicLinkHeader.h \
    WinHelp/TopicBlock.h \
    WinHelp/TopicBlockHeader.h \
    WinHelp/TopicPos.h \
    WinHelp/WinHelpTopicFile.h \
    WinHelp/WinHelpPhrImageFile.h \
    WinHelp/WinHelpPhrIndexFile.h \
    Utils/CompressionUtils.h \
    WinHelp/WinHelpPhraseFile.h \
    winhelpwindow.h \
    WinHelp/DLLMapsRecord.h \
    WinHelp/LanguageRecord.h \
    WinHelp/KeyindexRecord.h \
    WinHelp/IndexSRecord.h \
    WinHelp/GroupsRecord.h \
    WinHelp/FtindexRecord.h \
    WinHelp/DeffontRecord.h \
    WinHelp/CNTRecord.h \
    WinHelp/LCIDRecord.h \
    WinHelp/LCID.h \
    WinHelp/CitationRecord.h \
    Utils/TextUtils.h \
    Utils/DebugUtils.h \
    Utils/IOUtils.h \
    WinHelp/WindowMvpRecord.h \
    WinHelp/WindowHpjRecord.h \
    WinHelp/IconRecord.h \
    WinHelp/WinHelpIcon.h \
    WinHelp/ConfigRecord.h \
    WinHelp/ContentsRecord.h \
    WinHelp/TopicOffset.h \
    WinHelp/CopyrightRecord.h \
    WinHelp/TitleRecord.h \
    WinHelp/CharsetRecord.h \
    WinHelp/SystemRecordHeader.h \
    WinHelp/WinHelpSystemFile.h \
    WinHelp/SystemHeader.h \
    WinHelp/WinHelpFile.h \
    WinHelp/FreeHeader.h \
    WinHelp/WinHelpInternalDirectory.h \
    WinHelp/BTreeNodePage.h \
    WinHelp/BTreeNodeEntry.h \
    WinHelp/BTreeNodeHeader.h \
    WinHelp/BTreeIndexEntry.h \
    WinHelp/BTreeIndexPage.h \
    WinHelp/BTreeIndexHeader.h \
    WinHelp/BTreeHeader.h \
    WinHelp/InternalDirectoryFileHeader.h \
    WinHelp/WinHelpFileHeader.h \
    qwinhelp.h \
    helpbrowsingwidget.h \
    WinHelp/OldFontDescriptor.h \
    WinHelp/NewFontDescriptor.h \
    WinHelp/MVBFontDescriptor.h \
    WinHelp/WinHelpFontFile.h \
    WinHelp/FontDescriptor.h \
    WinHelp/FontStyle.h \
    WinHelp/WinHelpContextFile.h \
    WinHelp/ContextBTreeIndexEntry.h \
    WinHelp/ContextBTreeIndexPage.h \
    WinHelp/ContextBTreeNodeEntry.h \
    WinHelp/ContextBTreeNodePage.h \
    WinHelp/PictureHeader.h \
    WinHelp/PictureInfoHeader.h \
    WinHelp/BitmapHeader.h \
    WinHelp/MetafileHeader.h \
    WinHelp/WinHelpPicture.h \
    WinHelp/WinHelpPictureFile.h \
    WinHelp/WinHelpPicturesCollection.h
SOURCES += HelpReply.cpp \
    HelpAccessManager.cpp \
    HelpView.cpp \
    HelpIndexModel.cpp \
    WinHelp/TopicLink.cpp \
    WinHelp/TopicHeader.cpp \
    WinHelp/TopicRawDescriptor.cpp \
    WinHelp/DisplayableText.cpp \
    WinHelp/DisplayableTable.cpp \
    WinHelp/DisplayableTextNew.cpp \
    WinHelp/DisplayableTextOld.cpp \
    ThreadedWinHelpFileLoader.cpp \
    WinHelp/TopicHeaderNew.cpp \
    WinHelp/TopicHeaderOld.cpp \
    WinHelpApplication.cpp \
    WinHelp/TopicLinkHeader.cpp \
    WinHelp/TopicBlock.cpp \
    WinHelp/TopicBlockHeader.cpp \
    WinHelp/TopicPos.cpp \
    WinHelp/WinHelpTopicFile.cpp \
    WinHelp/WinHelpPhrImageFile.cpp \
    WinHelp/WinHelpPhrIndexFile.cpp \
    Utils/CompressionUtils.cpp \
    WinHelp/WinHelpPhraseFile.cpp \
    winhelpwindow.cpp \
    WinHelp/DLLMapsRecord.cpp \
    WinHelp/LanguageRecord.cpp \
    WinHelp/KeyindexRecord.cpp \
    WinHelp/IndexSRecord.cpp \
    WinHelp/GroupsRecord.cpp \
    WinHelp/FtindexRecord.cpp \
    WinHelp/DeffontRecord.cpp \
    WinHelp/CNTRecord.cpp \
    WinHelp/LCIDRecord.cpp \
    WinHelp/LCID.cpp \
    WinHelp/CitationRecord.cpp \
    Utils/TextUtils.cpp \
    Utils/IOUtils.cpp \
    WinHelp/WindowMvpRecord.cpp \
    WinHelp/WindowHpjRecord.cpp \
    WinHelp/IconRecord.cpp \
    WinHelp/WinHelpIcon.cpp \
    WinHelp/ConfigRecord.cpp \
    WinHelp/ContentsRecord.cpp \
    WinHelp/TopicOffset.cpp \
    WinHelp/CopyrightRecord.cpp \
    WinHelp/TitleRecord.cpp \
    WinHelp/CharsetRecord.cpp \
    WinHelp/SystemRecordHeader.cpp \
    WinHelp/WinHelpSystemFile.cpp \
    WinHelp/SystemHeader.cpp \
    WinHelp/WinHelpFile.cpp \
    WinHelp/FreeHeader.cpp \
    WinHelp/WinHelpInternalDirectory.cpp \
    WinHelp/BTreeNodePage.cpp \
    WinHelp/BTreeNodeEntry.cpp \
    WinHelp/BTreeNodeHeader.cpp \
    WinHelp/BTreeIndexEntry.cpp \
    WinHelp/BTreeIndexPage.cpp \
    WinHelp/BTreeIndexHeader.cpp \
    WinHelp/BTreeHeader.cpp \
    WinHelp/InternalDirectoryFileHeader.cpp \
    WinHelp/WinHelpFileHeader.cpp \
    main.cpp \
    qwinhelp.cpp \
    helpbrowsingwidget.cpp \
    WinHelp/OldFontDescriptor.cpp \
    WinHelp/NewFontDescriptor.cpp \
    WinHelp/MVBFontDescriptor.cpp \
    WinHelp/WinHelpFontFile.cpp \
    WinHelp/FontDescriptor.cpp \
    WinHelp/FontStyle.cpp \
    WinHelp/WinHelpContextFile.cpp \
    WinHelp/ContextBTreeIndexEntry.cpp \
    WinHelp/ContextBTreeIndexPage.cpp \
    WinHelp/ContextBTreeNodeEntry.cpp \
    WinHelp/ContextBTreeNodePage.cpp \
    WinHelp/PictureHeader.cpp \
    WinHelp/PictureInfoHeader.cpp \
    WinHelp/BitmapHeader.cpp \
    WinHelp/MetafileHeader.cpp \
    WinHelp/WinHelpPicture.cpp \
    WinHelp/WinHelpPictureFile.cpp \
    WinHelp/WinHelpPicturesCollection.cpp
FORMS += winhelpwindow.ui \
    qwinhelp.ui
RESOURCES +=
debug {
    DEFINES += DEBUG
}
QMAKE_CXXFLAGS_DEBUG += -std=c++0x
QMAKE_CXXFLAGS_RELEASE += -std=c++0x
QMAKE_CFLAGS_DEBUG += -std=c++0x
QMAKE_CFLAGS_RELEASE += -std=c++0x
