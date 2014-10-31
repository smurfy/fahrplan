#ifndef QTCREATOR_CASCADES_QML_COMPLETION
#define QTCREATOR_CASCADES_QML_COMPLETION

#include <bb/multimedia/AudioOutput>
#include <bb/multimedia/AudioRecorder>
#include <bb/multimedia/BufferStatus>
#include <bb/multimedia/Global>
#include <bb/multimedia/MediaError>
#include <bb/multimedia/MediaKey>
#include <bb/multimedia/MediaKeyNotification>
#include <bb/multimedia/MediaKeyWatcher>
#include <bb/multimedia/MediaPlayer>
#include <bb/multimedia/MediaState>
#include <bb/multimedia/MetaData>
#include <bb/multimedia/NowPlayingConnection>
#include <bb/multimedia/NowPlayingController>
#include <bb/multimedia/OverlayStyle>
#include <bb/multimedia/PriorityLevel>
#include <bb/multimedia/RepeatMode>
#include <bb/multimedia/ShuffleMode>
#include <bb/multimedia/SystemSound>
#include <bb/multimedia/VideoOutput>
#include <bb/network/Global>
#include <bb/network/PushCommand>
#include <bb/network/PushErrorCode>
#include <bb/network/PushPayload>
#include <bb/network/PushService>
#include <bb/network/PushStatus>
#include <bb/location/Global>
#include <bb/location/PositionErrorCode>
#include <bb/cascades/A11yComponentActivationEvent>
#include <bb/cascades/A11yComponentActivationType>
#include <bb/cascades/A11yMode>
#include <bb/cascades/A11yRole>
#include <bb/cascades/A11yState>
#include <bb/cascades/A11yStateHolder>
#include <bb/cascades/A11yValueAdjustmentType>
#include <bb/cascades/AbsoluteLayout>
#include <bb/cascades/AbsoluteLayoutProperties>
#include <bb/cascades/AbstractA11yObject>
#include <bb/cascades/AbstractA11ySpecialization>
#include <bb/cascades/AbstractActionItem>
#include <bb/cascades/AbstractAnimation>
#include <bb/cascades/AbstractButton>
#include <bb/cascades/AbstractContentSecurity>
#include <bb/cascades/AbstractCover>
#include <bb/cascades/AbstractDialog>
#include <bb/cascades/AbstractEventHandler>
#include <bb/cascades/AbstractGestureEvent>
#include <bb/cascades/AbstractPane>
#include <bb/cascades/AbstractShortcut>
#include <bb/cascades/AbstractTextControl>
#include <bb/cascades/AbstractTextFilter>
#include <bb/cascades/AbstractTextStyleRule>
#include <bb/cascades/AbstractTitleBarKindProperties>
#include <bb/cascades/AbstractToggleButton>
#include <bb/cascades/AbstractTransition>
#include <bb/cascades/ActionBarAutoHideBehavior>
#include <bb/cascades/ActionBarFollowKeyboardPolicy>
#include <bb/cascades/ActionBarPlacement>
#include <bb/cascades/ActionItem>
#include <bb/cascades/ActionMenuVisualState>
#include <bb/cascades/ActionSet>
#include <bb/cascades/ActiveTextHandler>
#include <bb/cascades/ActiveTextTriggeredEvent>
#include <bb/cascades/ActivityIndicator>
#include <bb/cascades/AdaptableExtensionPlugin>
#include <bb/cascades/AnimationRepeatCount>
#include <bb/cascades/AnimationState>
#include <bb/cascades/Application>
#include <bb/cascades/ApplicationViewCover>
#include <bb/cascades/ArrayDataModel>
#include <bb/cascades/BaseObject>
#include <bb/cascades/Button>
#include <bb/cascades/CheckBox>
#include <bb/cascades/ChromeVisibility>
#include <bb/cascades/CollapseMode>
#include <bb/cascades/Color>
#include <bb/cascades/ColorPaint>
#include <bb/cascades/ColorTheme>
#include <bb/cascades/ComponentA11ySpecialization>
#include <bb/cascades/Container>
#include <bb/cascades/ContentSecurity>
#include <bb/cascades/ContextMenuHandler>
#include <bb/cascades/ContextMenuShowingEvent>
#include <bb/cascades/ContextMenuVisualState>
#include <bb/cascades/Control>
#include <bb/cascades/ControlAppearance>
#include <bb/cascades/CoverDetailLevel>
#include <bb/cascades/CustomA11yObject>
#include <bb/cascades/CustomControl>
#include <bb/cascades/CustomListItem>
#include <bb/cascades/DataModel>
#include <bb/cascades/DataModelChangeType>
#include <bb/cascades/DateTimePicker>
#include <bb/cascades/DateTimePickerMode>
#include <bb/cascades/DeleteActionItem>
#include <bb/cascades/DevelopmentSupport>
#include <bb/cascades/Dialog>
#include <bb/cascades/DisplayDirection>
#include <bb/cascades/Divider>
#include <bb/cascades/DockLayout>
#include <bb/cascades/DoubleTapEvent>
#include <bb/cascades/DoubleTapHandler>
#include <bb/cascades/DropDown>
#include <bb/cascades/EasingCurve>
#include <bb/cascades/Event>
#include <bb/cascades/EventReaction>
#include <bb/cascades/ExpandableView>
#include <bb/cascades/ExpandMode>
#include <bb/cascades/FadeTransition>
#include <bb/cascades/FlickMode>
#include <bb/cascades/FlowListLayout>
#include <bb/cascades/FlowListLayoutProperties>
#include <bb/cascades/FocusAutoShow>
#include <bb/cascades/FocusPolicy>
#include <bb/cascades/FocusRetentionPolicy>
#include <bb/cascades/FontFaceRule>
#include <bb/cascades/FontSize>
#include <bb/cascades/FontStyle>
#include <bb/cascades/FontWeight>
#include <bb/cascades/ForeignWindowControl>
#include <bb/cascades/FreeFormTitleBarKindProperties>
#include <bb/cascades/GestureHandler>
#include <bb/cascades/GridLayout>
#include <bb/cascades/GridListLayout>
#include <bb/cascades/GroupAnimation>
#include <bb/cascades/GroupDataModel>
#include <bb/cascades/Header>
#include <bb/cascades/HeaderMode>
#include <bb/cascades/HelpActionItem>
#include <bb/cascades/HighlightAppearance>
#include <bb/cascades/HorizontalAlignment>
#include <bb/cascades/Image>
#include <bb/cascades/ImageAnimator>
#include <bb/cascades/ImageAnimatorAnchor>
#include <bb/cascades/ImageAnimatorSeekMode>
#include <bb/cascades/ImageButton>
#include <bb/cascades/ImagePaint>
#include <bb/cascades/ImageToggleButton>
#include <bb/cascades/ImageTracker>
#include <bb/cascades/ImageView>
#include <bb/cascades/ImageViewLoadEffect>
#include <bb/cascades/ImplicitAnimationController>
#include <bb/cascades/InputRouteProperties>
#include <bb/cascades/Invocation>
#include <bb/cascades/InvokeActionItem>
#include <bb/cascades/InvokeHandler>
#include <bb/cascades/InvokeQuery>
#include <bb/cascades/InvokeState>
#include <bb/cascades/ItemGrouping>
#include <bb/cascades/JavaScriptWorld>
#include <bb/cascades/KeyEvent>
#include <bb/cascades/KeyLayout>
#include <bb/cascades/KeyListener>
#include <bb/cascades/KeyModifier>
#include <bb/cascades/Label>
#include <bb/cascades/LabelAutoSizeProperties>
#include <bb/cascades/LabelTextFitMode>
#include <bb/cascades/LabelTextFitProperties>
#include <bb/cascades/Layout>
#include <bb/cascades/LayoutOrientation>
#include <bb/cascades/LayoutProperties>
#include <bb/cascades/LayoutUpdateHandler>
#include <bb/cascades/ListHeaderMode>
#include <bb/cascades/ListItemListener>
#include <bb/cascades/ListItemProvider>
#include <bb/cascades/ListItemTypeMapper>
#include <bb/cascades/ListLayout>
#include <bb/cascades/ListRearrangeHandler>
#include <bb/cascades/ListScrollStateHandler>
#include <bb/cascades/ListView>
#include <bb/cascades/ListViewStickToEdgePolicy>
#include <bb/cascades/LocaleHandler>
#include <bb/cascades/LongPressEvent>
#include <bb/cascades/LongPressHandler>
#include <bb/cascades/Menu>
#include <bb/cascades/MultiCover>
#include <bb/cascades/MultiSelectActionItem>
#include <bb/cascades/MultiSelectHandler>
#include <bb/cascades/NavigationPane>
#include <bb/cascades/NavigationPaneProperties>
#include <bb/cascades/Option>
#include <bb/cascades/OrientationSupport>
#include <bb/cascades/OverlapTouchPolicy>
#include <bb/cascades/OverScrollEffectMode>
#include <bb/cascades/Page>
#include <bb/cascades/PageLayoutUpdateHandler>
#include <bb/cascades/PageResizeBehavior>
#include <bb/cascades/Paint>
#include <bb/cascades/PaneProperties>
#include <bb/cascades/ParallelAnimation>
#include <bb/cascades/Picker>
#include <bb/cascades/PickerKind>
#include <bb/cascades/PickerProvider>
#include <bb/cascades/PinchEvent>
#include <bb/cascades/PinchHandler>
#include <bb/cascades/ProgressIndicator>
#include <bb/cascades/ProgressIndicatorState>
#include <bb/cascades/PropagationPhase>
#include <bb/cascades/QListDataModel>
#include <bb/cascades/QmlDocument>
#include <bb/cascades/RadioGroup>
#include <bb/cascades/RenderFence>
#include <bb/cascades/RenderFenceState>
#include <bb/cascades/RepeatPattern>
#include <bb/cascades/Resource>
#include <bb/cascades/ResourceState>
#include <bb/cascades/ResourceTracker>
#include <bb/cascades/RotateTransition>
#include <bb/cascades/ScaleTransition>
#include <bb/cascades/ScalingMethod>
#include <bb/cascades/SceneCover>
#include <bb/cascades/ScreenIdleMode>
#include <bb/cascades/ScrollAnimation>
#include <bb/cascades/ScrollIndicatorMode>
#include <bb/cascades/ScrollMode>
#include <bb/cascades/ScrollPosition>
#include <bb/cascades/ScrollRailsPolicy>
#include <bb/cascades/ScrollRole>
#include <bb/cascades/ScrollView>
#include <bb/cascades/ScrollViewProperties>
#include <bb/cascades/SegmentedControl>
#include <bb/cascades/SequentialAnimation>
#include <bb/cascades/SettingsActionItem>
#include <bb/cascades/Sheet>
#include <bb/cascades/Shortcut>
#include <bb/cascades/ShortcutTriggerMode>
#include <bb/cascades/SidebarState>
#include <bb/cascades/SidebarVisualState>
#include <bb/cascades/SingleLineTextFilter>
#include <bb/cascades/Slider>
#include <bb/cascades/SnapMode>
#include <bb/cascades/Spacings>
#include <bb/cascades/StackLayout>
#include <bb/cascades/StackLayoutProperties>
#include <bb/cascades/StackListLayout>
#include <bb/cascades/StandardListItem>
#include <bb/cascades/StandardPickerItem>
#include <bb/cascades/StockCurve>
#include <bb/cascades/SubmitKey>
#include <bb/cascades/SubmitKeyFocusBehavior>
#include <bb/cascades/SupportedDisplayOrientation>
#include <bb/cascades/SystemDefaults>
#include <bb/cascades/SystemShortcut>
#include <bb/cascades/SystemShortcuts>
#include <bb/cascades/Tab>
#include <bb/cascades/TabbedPane>
#include <bb/cascades/TapEvent>
#include <bb/cascades/TapHandler>
#include <bb/cascades/TextAlign>
#include <bb/cascades/TextArea>
#include <bb/cascades/TextAreaAutoSizeProperties>
#include <bb/cascades/TextAreaInputMode>
#include <bb/cascades/TextAreaScrollMode>
#include <bb/cascades/TextAreaTextFitProperties>
#include <bb/cascades/TextAutoFit>
#include <bb/cascades/TextContentFlag>
#include <bb/cascades/TextContentProperties>
#include <bb/cascades/TextEditor>
#include <bb/cascades/TextField>
#include <bb/cascades/TextFieldInputMode>
#include <bb/cascades/TextFieldTextFitProperties>
#include <bb/cascades/TextFieldTitleBarKindProperties>
#include <bb/cascades/TextFitProperties>
#include <bb/cascades/TextFormat>
#include <bb/cascades/TextInputActionItem>
#include <bb/cascades/TextInputFlag>
#include <bb/cascades/TextInputMasking>
#include <bb/cascades/TextInputProperties>
#include <bb/cascades/TextStyle>
#include <bb/cascades/TextStyleDefinition>
#include <bb/cascades/Theme>
#include <bb/cascades/ThemeSupport>
#include <bb/cascades/TitleBar>
#include <bb/cascades/TitleBarAppearance>
#include <bb/cascades/TitleBarExpandableArea>
#include <bb/cascades/TitleBarExpandableAreaIndicatorVisibility>
#include <bb/cascades/TitleBarExpandableAreaToggleArea>
#include <bb/cascades/TitleBarKind>
#include <bb/cascades/TitleBarScrollBehavior>
#include <bb/cascades/ToggleButton>
#include <bb/cascades/TouchBehavior>
#include <bb/cascades/TouchEnterEvent>
#include <bb/cascades/TouchEvent>
#include <bb/cascades/TouchExitEvent>
#include <bb/cascades/TouchKeyboardEvent>
#include <bb/cascades/TouchKeyboardHandler>
#include <bb/cascades/TouchPropagationMode>
#include <bb/cascades/TouchReaction>
#include <bb/cascades/TouchResponse>
#include <bb/cascades/TouchType>
#include <bb/cascades/TranslateTransition>
#include <bb/cascades/UIConfig>
#include <bb/cascades/UIObject>
#include <bb/cascades/UIOrientation>
#include <bb/cascades/UIPalette>
#include <bb/cascades/ValidationMode>
#include <bb/cascades/ValidationState>
#include <bb/cascades/Validator>
#include <bb/cascades/ValueA11ySpecialization>
#include <bb/cascades/VerticalAlignment>
#include <bb/cascades/VisualNode>
#include <bb/cascades/VisualStyle>
#include <bb/cascades/WebCookieJar>
#include <bb/cascades/WebDownloadRequest>
#include <bb/cascades/WebFindFlag>
#include <bb/cascades/WebInputProperties>
#include <bb/cascades/WebLoadRequest>
#include <bb/cascades/WebLoadStatus>
#include <bb/cascades/WebNavigationRequest>
#include <bb/cascades/WebNavigationRequestAction>
#include <bb/cascades/WebNavigationType>
#include <bb/cascades/WebNewViewRequest>
#include <bb/cascades/WebPage>
#include <bb/cascades/WebPageCompositor>
#include <bb/cascades/WebPageEventHandler>
#include <bb/cascades/WebPageTexture>
#include <bb/cascades/WebResourceRequest>
#include <bb/cascades/WebResourceRequestFilter>
#include <bb/cascades/WebSettings>
#include <bb/cascades/WebStorage>
#include <bb/cascades/WebView>
#include <bb/cascades/Window>
#include <bb/cascades/WindowProperty>
#include <bb/cascades/WindowState>
#include <bb/cascades/XmlDataModel>
#include <bb/cascades/multimedia/BarcodeDetector>
#include <bb/cascades/multimedia/BarcodeDetectorVisuals>
#include <bb/cascades/multimedia/BarcodeFormat>
#include <bb/cascades/multimedia/BarcodeGenerator>
#include <bb/cascades/multimedia/BarcodeGeneratorError>
#include <bb/cascades/multimedia/BarcodeGeneratorFormat>
#include <bb/cascades/multimedia/BarcodeView>
#include <bb/cascades/multimedia/Camera>
#include <bb/cascades/multimedia/CameraError>
#include <bb/cascades/multimedia/CameraFlashMode>
#include <bb/cascades/multimedia/CameraFocusMode>
#include <bb/cascades/multimedia/CameraFocusState>
#include <bb/cascades/multimedia/CameraMode>
#include <bb/cascades/multimedia/CameraSceneMode>
#include <bb/cascades/multimedia/CameraSettings>
#include <bb/cascades/multimedia/CameraShootingMode>
#include <bb/cascades/multimedia/CameraUnit>
#include <bb/cascades/multimedia/Global>
#include <bb/cascades/multimedia/QrCodeErrorCorrection>
#include <bb/cascades/multimedia/QrCodeRenderResult>
#include <bb/cascades/multimedia/QrCodeView>
#include <bb/cascades/places/Global>
#include <bb/cascades/places/PlacePicker>
#include <bb/cascades/places/PlacePickerControl>
#include <bb/cascades/places/SelectedPlace>
#include <bb/cascades/places/SelectedPlaceImageType>
#include <bb/cascades/advertisement/Banner>
#include <bb/cascades/advertisement/Global>
#include <bb/cascades/pickers/CalendarEventPicker>
#include <bb/cascades/pickers/CalendarEventPickerError>
#include <bb/cascades/pickers/ContactPicker>
#include <bb/cascades/pickers/ContactSelectionMode>
#include <bb/cascades/pickers/FilePicker>
#include <bb/cascades/pickers/FilePickerError>
#include <bb/cascades/pickers/FilePickerMode>
#include <bb/cascades/pickers/FilePickerSortFlag>
#include <bb/cascades/pickers/FilePickerSortOrder>
#include <bb/cascades/pickers/FilePickerSourceRestriction>
#include <bb/cascades/pickers/FilePickerViewMode>
#include <bb/cascades/pickers/FileType>
#include <bb/cascades/pickers/Global>
#include <bb/cascades/pickers/ICalendarConstants>
#include <bb/cascades/pickers/SelectedContactAttribute>
#include <bb/cascades/datamanager/AsyncDataModel>
#include <bb/cascades/datamanager/AsyncHeaderDataModel>
#include <bb/cascades/datamanager/AsyncHierarchicalDataModel>
#include <bb/cascades/datamanager/DataItem>
#include <bb/cascades/datamanager/DataModelDecorator>
#include <bb/cascades/datamanager/DataQuery>
#include <bb/cascades/datamanager/DataQueryDecorator>
#include <bb/cascades/datamanager/DataRevision>
#include <bb/cascades/datamanager/Global>
#include <bb/cascades/datamanager/HeaderDataItem>
#include <bb/cascades/datamanager/HeaderDataQuery>
#include <bb/cascades/datamanager/NumericRevision>
#include <bb/cascades/datamanager/QueryDataModel>
#include <bb/cascades/datamanager/Revision>
#include <bb/cascades/datamanager/SimpleQueryDataModel>
#include <bb/cascades/datamanager/SqlDataQuery>
#include <bb/cascades/datamanager/SqlHeaderDataQuery>
#include <bb/cascades/datamanager/SqlQueryUtils>
#include <bb/cascades/maps/BlankRenderEngine>
#include <bb/cascades/maps/DataProvider>
#include <bb/cascades/maps/GeographicsProvisioningBridge>
#include <bb/cascades/maps/GeographicsProvisioningBridgeDataSource>
#include <bb/cascades/maps/GeographicsProvisioningBridgeResult>
#include <bb/cascades/maps/Global>
#include <bb/cascades/maps/KMLConversion>
#include <bb/cascades/maps/MapCoverageChecker>
#include <bb/cascades/maps/MapData>
#include <bb/cascades/maps/MapFactory>
#include <bb/cascades/maps/MapImageCacheManager>
#include <bb/cascades/maps/MapImageGenerator>
#include <bb/cascades/maps/MapImageGeneratorError>
#include <bb/cascades/maps/MapLongPressToPinDrop>
#include <bb/cascades/maps/MapView>
#include <bb/cascades/maps/RenderEngine>
#include <bb/cascades/maps/RenderEngineConfiguration>
#include <bb/cascades/maps/RenderEngineInfo>
#include <bb/cascades/maps/ViewProperties>
#include <bb/pim/Global>
#include <bb/pim/notebook/Notebook>
#include <bb/pim/notebook/NotebookConstraint>
#include <bb/pim/notebook/NotebookDomain>
#include <bb/pim/notebook/NotebookEntry>
#include <bb/pim/notebook/NotebookEntryAttachment>
#include <bb/pim/notebook/NotebookEntryConstraint>
#include <bb/pim/notebook/NotebookEntryDescription>
#include <bb/pim/notebook/NotebookEntryDetailLevel>
#include <bb/pim/notebook/NotebookEntryDueDate>
#include <bb/pim/notebook/NotebookEntryFilter>
#include <bb/pim/notebook/NotebookEntryHeader>
#include <bb/pim/notebook/NotebookEntryId>
#include <bb/pim/notebook/NotebookEntrySearchField>
#include <bb/pim/notebook/NotebookEntrySortColumn>
#include <bb/pim/notebook/NotebookEntrySortOrder>
#include <bb/pim/notebook/NotebookEntryStatus>
#include <bb/pim/notebook/NotebookFilter>
#include <bb/pim/notebook/NotebookId>
#include <bb/pim/notebook/NotebookService>
#include <bb/pim/notebook/NotebookServiceResult>
#include <bb/pim/contacts/Contact>
#include <bb/pim/contacts/ContactActivity>
#include <bb/pim/contacts/ContactAttribute>
#include <bb/pim/contacts/ContactAttributeBuilder>
#include <bb/pim/contacts/ContactAutoCompleteSearchFilters>
#include <bb/pim/contacts/ContactBuilder>
#include <bb/pim/contacts/ContactConsts>
#include <bb/pim/contacts/ContactFavouriteAction>
#include <bb/pim/contacts/ContactFavouriteActionBuilder>
#include <bb/pim/contacts/ContactGroup>
#include <bb/pim/contacts/ContactGroupBuilder>
#include <bb/pim/contacts/ContactListFilters>
#include <bb/pim/contacts/ContactNews>
#include <bb/pim/contacts/ContactOnlineStatus>
#include <bb/pim/contacts/ContactPhoto>
#include <bb/pim/contacts/ContactPhotoBuilder>
#include <bb/pim/contacts/ContactPostalAddress>
#include <bb/pim/contacts/ContactPostalAddressBuilder>
#include <bb/pim/contacts/ContactRemoteSearchFilters>
#include <bb/pim/contacts/ContactSearchFilters>
#include <bb/pim/contacts/ContactService>
#include <bb/pim/calendar/AnalyticsParams>
#include <bb/pim/calendar/AnalyticsStats>
#include <bb/pim/calendar/Attendee>
#include <bb/pim/calendar/AttendeeInEvents>
#include <bb/pim/calendar/AttendeeRole>
#include <bb/pim/calendar/AttendeeSearchParameters>
#include <bb/pim/calendar/AttendeeStatus>
#include <bb/pim/calendar/BbmConference>
#include <bb/pim/calendar/BbmConferencePreferredData>
#include <bb/pim/calendar/BbmConferenceUserData>
#include <bb/pim/calendar/BusyStatus>
#include <bb/pim/calendar/CalendarEvent>
#include <bb/pim/calendar/CalendarFolder>
#include <bb/pim/calendar/CalendarService>
#include <bb/pim/calendar/CalendarSettings>
#include <bb/pim/calendar/CommonAttendee>
#include <bb/pim/calendar/CommonLocation>
#include <bb/pim/calendar/DataTypes>
#include <bb/pim/calendar/DayOfWeek>
#include <bb/pim/calendar/DetailLevel>
#include <bb/pim/calendar/EventKey>
#include <bb/pim/calendar/EventRefresh>
#include <bb/pim/calendar/EventSearchParameters>
#include <bb/pim/calendar/FolderKey>
#include <bb/pim/calendar/Frequency>
#include <bb/pim/calendar/ICalendarData>
#include <bb/pim/calendar/ICalendarDataPath>
#include <bb/pim/calendar/ICalendarObjects>
#include <bb/pim/calendar/ICalendarTodo>
#include <bb/pim/calendar/ICalendarTodoPriority>
#include <bb/pim/calendar/ICalendarTodoStatus>
#include <bb/pim/calendar/MeetingHistory>
#include <bb/pim/calendar/Notification>
#include <bb/pim/calendar/OnlineConferenceData>
#include <bb/pim/calendar/Recurrence>
#include <bb/pim/calendar/Result>
#include <bb/pim/calendar/Sensitivity>
#include <bb/pim/calendar/SortField>
#include <bb/pim/calendar/WeekStartDay>
#include <bb/pim/message/Attachment>
#include <bb/pim/message/AttachmentDownloadStatus>
#include <bb/pim/message/AttachmentEncoding>
#include <bb/pim/message/AttachmentFilter>
#include <bb/pim/message/Conversation>
#include <bb/pim/message/ConversationBuilder>
#include <bb/pim/message/ConversationStatus>
#include <bb/pim/message/ConversationUpdate>
#include <bb/pim/message/FileAttachment>
#include <bb/pim/message/Filter>
#include <bb/pim/message/Folder>
#include <bb/pim/message/FollowupFlag>
#include <bb/pim/message/Keys>
#include <bb/pim/message/MarkOrDeletePriorFilter>
#include <bb/pim/message/MarkOrDeletePriorOp>
#include <bb/pim/message/Message>
#include <bb/pim/message/MessageBody>
#include <bb/pim/message/MessageBuilder>
#include <bb/pim/message/MessageClass>
#include <bb/pim/message/MessageContact>
#include <bb/pim/message/MessageFilter>
#include <bb/pim/message/MessageFlag>
#include <bb/pim/message/MessageFolder>
#include <bb/pim/message/MessagePriority>
#include <bb/pim/message/MessageReplyState>
#include <bb/pim/message/MessageRights>
#include <bb/pim/message/MessageSearchFilter>
#include <bb/pim/message/MessageService>
#include <bb/pim/message/MessageServiceFeature>
#include <bb/pim/message/MessageStatus>
#include <bb/pim/message/MessageTransaction>
#include <bb/pim/message/MessageTransmissionStatus>
#include <bb/pim/message/MessageUpdate>
#include <bb/pim/message/MessageUpdateFilter>
#include <bb/pim/message/MessageUpdateType>
#include <bb/pim/message/MimeTypes>
#include <bb/pim/message/OutOfOfficeSettings>
#include <bb/pim/message/SearchFilterCriteria>
#include <bb/pim/message/SearchStatusCriteria>
#include <bb/pim/message/SmsTransport>
#include <bb/pim/message/SmsTransportRegistrationResult>
#include <bb/pim/message/SmsTransportSendResult>
#include <bb/pim/message/StatusReport>
#include <bb/pim/message/StatusReportType>
#include <bb/pim/common/ContactEntry>
#include <bb/pim/phone/CallAttribute>
#include <bb/pim/phone/CallEntry>
#include <bb/pim/phone/CallEntryResult>
#include <bb/pim/phone/CallHistoryError>
#include <bb/pim/phone/CallHistoryFilter>
#include <bb/pim/phone/CallHistoryParam>
#include <bb/pim/phone/CallHistoryService>
#include <bb/pim/phone/CallType>
#include <bb/pim/phone/SortOrder>
#include <bb/pim/account/Account>
#include <bb/pim/account/AccountsChanged>
#include <bb/pim/account/AccountService>
#include <bb/pim/account/Property>
#include <bb/pim/account/Provider>
#include <bb/pim/account/Result>
#include <bb/pim/account/Service>
#include <bb/utility/Global>
#include <bb/utility/ImageConverter>
#include <bb/utility/SpellChecker>
#include <bb/utility/StringValidator>
#include <bb/utility/i18n/ClockFormat>
#include <bb/utility/i18n/CustomDateFormatter>
#include <bb/utility/i18n/DateFormat>
#include <bb/utility/i18n/Formatting>
#include <bb/utility/i18n/Global>
#include <bb/utility/i18n/RelativeDateFormat>
#include <bb/utility/i18n/RelativeDateFormatter>
#include <bb/system/ApplicationStartupMode>
#include <bb/system/CardDoneMessage>
#include <bb/system/CardEdge>
#include <bb/system/CardOrientation>
#include <bb/system/CardPeek>
#include <bb/system/CardResizeMessage>
#include <bb/system/Clipboard>
#include <bb/system/ContextActivityState>
#include <bb/system/ContextClassification>
#include <bb/system/ContextCollector>
#include <bb/system/ContextEvent>
#include <bb/system/FileTransferMode>
#include <bb/system/Global>
#include <bb/system/InvokeAction>
#include <bb/system/InvokeDateTime>
#include <bb/system/InvokeListCursorDirection>
#include <bb/system/InvokeManager>
#include <bb/system/InvokeQueryTargetsReply>
#include <bb/system/InvokeQueryTargetsRequest>
#include <bb/system/InvokeRecurrenceRule>
#include <bb/system/InvokeRecurrenceRuleFrequency>
#include <bb/system/InvokeRecurrenceRuleLimit>
#include <bb/system/InvokeReply>
#include <bb/system/InvokeReplyError>
#include <bb/system/InvokeRequest>
#include <bb/system/InvokeRequestTargetFiltersReply>
#include <bb/system/InvokeSource>
#include <bb/system/InvokeTarget>
#include <bb/system/InvokeTargetFilter>
#include <bb/system/InvokeTargetReply>
#include <bb/system/InvokeTimerRequest>
#include <bb/system/InvokeUpdateTargetFiltersRequest>
#include <bb/system/ListSelectionIndicator>
#include <bb/system/ListSelectionMode>
#include <bb/system/LocaleHandler>
#include <bb/system/LocaleType>
#include <bb/system/MeasurementSystem>
#include <bb/system/Menu>
#include <bb/system/MenuItem>
#include <bb/system/MenuItemInvokeParams>
#include <bb/system/MenuItemType>
#include <bb/system/MenuManager>
#include <bb/system/MenuManagerError>
#include <bb/system/NfcShareDataContent>
#include <bb/system/NfcShareError>
#include <bb/system/NfcShareFilesContent>
#include <bb/system/NfcShareHandoverTransport>
#include <bb/system/NfcShareManager>
#include <bb/system/NfcShareMode>
#include <bb/system/NfcShareSetContentError>
#include <bb/system/NfcShareStartTransferMode>
#include <bb/system/NfcShareSuccess>
#include <bb/system/Screenshot>
#include <bb/system/ScreenshotError>
#include <bb/system/ScreenshotFormat>
#include <bb/system/SecurityPerimeter>
#include <bb/system/SystemCredentialsPrompt>
#include <bb/system/SystemDialog>
#include <bb/system/SystemListDialog>
#include <bb/system/SystemProgressDialog>
#include <bb/system/SystemProgressToast>
#include <bb/system/SystemPrompt>
#include <bb/system/SystemToast>
#include <bb/system/SystemUiButton>
#include <bb/system/SystemUiError>
#include <bb/system/SystemUiInputField>
#include <bb/system/SystemUiInputMode>
#include <bb/system/SystemUiInputOption>
#include <bb/system/SystemUiModality>
#include <bb/system/SystemUiPosition>
#include <bb/system/SystemUiProgressState>
#include <bb/system/SystemUiResult>
#include <bb/system/SystemUiReturnKeyAction>
#include <bb/system/phone/Call>
#include <bb/system/phone/CallAction>
#include <bb/system/phone/CallCommandResponse>
#include <bb/system/phone/CallState>
#include <bb/system/phone/CallType>
#include <bb/system/phone/Line>
#include <bb/system/phone/LineType>
#include <bb/system/phone/Phone>
#include <bb/device/BatteryChargingState>
#include <bb/device/BatteryCondition>
#include <bb/device/BatteryInfo>
#include <bb/device/CellularBand>
#include <bb/device/CellularConnectionRequestResult>
#include <bb/device/CellularConnectionState>
#include <bb/device/CellularDataInterface>
#include <bb/device/CellularNetworkInfo>
#include <bb/device/CellularRadioInfo>
#include <bb/device/CellularService>
#include <bb/device/CellularTechnology>
#include <bb/device/DeviceInfo>
#include <bb/device/DeviceOrientation>
#include <bb/device/DisplayAspectType>
#include <bb/device/DisplayInfo>
#include <bb/device/DisplayTechnology>
#include <bb/device/Global>
#include <bb/device/HardwareInfo>
#include <bb/device/HdmiConnector>
#include <bb/device/Led>
#include <bb/device/LedColor>
#include <bb/device/NetworkDataUsage>
#include <bb/device/SdCardActivity>
#include <bb/device/SdCardInfo>
#include <bb/device/SdCardState>
#include <bb/device/SimCardInfo>
#include <bb/device/SimCardState>
#include <bb/device/UserActivityState>
#include <bb/device/VibrationController>
#include <bb/device/WiFiDirect>
#include <bb/device/WiFiDirectConnectionState>
#include <bb/device/WiFiDirectDeviceType>
#include <bb/device/WiFiDirectIntent>
#include <bb/device/WiFiDirectResult>
#include <bb/device/WiFiDirectRole>
#include <bb/device/WiFiDirectSecondaryDeviceType>
#include <bb/device/WiFiDirectWpsMethod>
#include <bb/data/AsyncDataAccess>
#include <bb/data/AsyncWorker>
#include <bb/data/DataAccessError>
#include <bb/data/DataAccessErrorType>
#include <bb/data/DataAccessReply>
#include <bb/data/DataSource>
#include <bb/data/DataSourceType>
#include <bb/data/JsonDataAccess>
#include <bb/data/SqlConnection>
#include <bb/data/SqlDataAccess>
#include <bb/data/SqlWorker>
#include <bb/data/XmlDataAccess>
#include <bb/platform/CancelSubscriptionReply>
#include <bb/platform/DeviceLockState>
#include <bb/platform/DigitalGoodReply>
#include <bb/platform/DigitalGoodState>
#include <bb/platform/ExistingPurchasesReply>
#include <bb/platform/Global>
#include <bb/platform/HandsFreeNavigation>
#include <bb/platform/HomeScreen>
#include <bb/platform/LocationMapInvoker>
#include <bb/platform/MapInvoker>
#include <bb/platform/MapNavigationMode>
#include <bb/platform/MapSearchMode>
#include <bb/platform/MapTransportationMode>
#include <bb/platform/Notification>
#include <bb/platform/NotificationApplicationSettings>
#include <bb/platform/NotificationDefaultApplicationSettings>
#include <bb/platform/NotificationDialog>
#include <bb/platform/NotificationError>
#include <bb/platform/NotificationGlobalSettings>
#include <bb/platform/NotificationMode>
#include <bb/platform/NotificationPolicy>
#include <bb/platform/NotificationPriorityPolicy>
#include <bb/platform/NotificationResult>
#include <bb/platform/NotificationSettingsError>
#include <bb/platform/NotificationType>
#include <bb/platform/PaymentConnectionMode>
#include <bb/platform/PaymentErrorCode>
#include <bb/platform/PaymentManager>
#include <bb/platform/PaymentReply>
#include <bb/platform/PlatformInfo>
#include <bb/platform/PriceReply>
#include <bb/platform/PurchaseReceipt>
#include <bb/platform/PurchaseReply>
#include <bb/platform/RouteMapInvoker>
#include <bb/platform/SubscriptionStatusReply>
#include <bb/platform/SubscriptionTermsReply>
#include <bb/platform/TypicalMapAltitude>
#include <bb/platform/WallpaperLockState>
#include <bb/platform/WallpaperResult>
#include <bb/platform/identity/BlackBerryId>
#include <bb/platform/identity/BlackBerryIdAssuranceLevel>
#include <bb/platform/identity/BlackBerryIdChallengeFlag>
#include <bb/platform/identity/BlackBerryIdChallengeType>
#include <bb/platform/identity/BlackBerryIdPropertyType>
#include <bb/platform/identity/BlackBerryProfile>
#include <bb/platform/identity/BlackBerryProfileCreateDataFlag>
#include <bb/platform/identity/BlackBerryProfileDeleteDataFlag>
#include <bb/platform/identity/BlackBerryProfileGetDataFlag>
#include <bb/platform/identity/BlackBerryProfileListDataFlag>
#include <bb/platform/identity/BlackBerryProfileNotificationFlag>
#include <bb/platform/identity/BlackBerryProfilePropertyType>
#include <bb/platform/identity/BlackBerryProfileRegisterNotifierFlag>
#include <bb/platform/identity/BlackBerryProfileSetDataFlag>
#include <bb/platform/identity/IdentityService>
#include <bb/platform/identity/IdentityServiceLogVerbosity>
#include <bb/platform/identity/IdentityServicePropertyResult>
#include <bb/platform/identity/IdentityServiceProvider>
#include <bb/platform/identity/IdentityServiceResult>
#include <bb/platform/identity/IdentityServiceTokenResult>
#include <bb/platform/bbm/ApplicationPermissions>
#include <bb/platform/bbm/Contact>
#include <bb/platform/bbm/ContactService>
#include <bb/platform/bbm/Context>
#include <bb/platform/bbm/Global>
#include <bb/platform/bbm/ImageType>
#include <bb/platform/bbm/MessageService>
#include <bb/platform/bbm/Permission>
#include <bb/platform/bbm/PermissionValue>
#include <bb/platform/bbm/ProfileBox>
#include <bb/platform/bbm/ProfileBoxItem>
#include <bb/platform/bbm/RegistrationState>
#include <bb/platform/bbm/UserProfile>
#include <bb/platform/bbm/UserStatus>
#include <bb/platform/geo/AltitudeMode>
#include <bb/platform/geo/BoundingBox>
#include <bb/platform/geo/Coordinate>
#include <bb/platform/geo/EdgeSize>
#include <bb/platform/geo/EdgeStyle>
#include <bb/platform/geo/GeoDeviceLocation>
#include <bb/platform/geo/Geographic>
#include <bb/platform/geo/GeoList>
#include <bb/platform/geo/GeoLocation>
#include <bb/platform/geo/GeoPolygon>
#include <bb/platform/geo/GeoPolyline>
#include <bb/platform/geo/GeoShape>
#include <bb/platform/geo/Global>
#include <bb/platform/geo/Marker>
#include <bb/platform/geo/Point>
#include <bb/platform/geo/Polyline>
#include <bb/platform/geo/Style>
#include <bb/platform/geo/StyleSheet>


void registerCascadesQMLTypes()
{
	qmlRegisterType<bb::multimedia::AudioOutput>("bb.cascades",1,3,"AudioOutput");
	qmlRegisterType<bb::multimedia::AudioRecorder>("bb.cascades",1,3,"AudioRecorder");
	qmlRegisterType<bb::multimedia::BufferStatus>("bb.cascades",1,3,"BufferStatus");
	qmlRegisterType<bb::multimedia::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::multimedia::MediaError>("bb.cascades",1,3,"MediaError");
	qmlRegisterType<bb::multimedia::MediaKey>("bb.cascades",1,3,"MediaKey");
	qmlRegisterType<bb::multimedia::MediaKeyNotification>("bb.cascades",1,3,"MediaKeyNotification");
	qmlRegisterType<bb::multimedia::MediaKeyWatcher>("bb.cascades",1,3,"MediaKeyWatcher");
	qmlRegisterType<bb::multimedia::MediaPlayer>("bb.cascades",1,3,"MediaPlayer");
	qmlRegisterType<bb::multimedia::MediaState>("bb.cascades",1,3,"MediaState");
	qmlRegisterType<bb::multimedia::MetaData>("bb.cascades",1,3,"MetaData");
	qmlRegisterType<bb::multimedia::NowPlayingConnection>("bb.cascades",1,3,"NowPlayingConnection");
	qmlRegisterType<bb::multimedia::NowPlayingController>("bb.cascades",1,3,"NowPlayingController");
	qmlRegisterType<bb::multimedia::OverlayStyle>("bb.cascades",1,3,"OverlayStyle");
	qmlRegisterType<bb::multimedia::PriorityLevel>("bb.cascades",1,3,"PriorityLevel");
	qmlRegisterType<bb::multimedia::RepeatMode>("bb.cascades",1,3,"RepeatMode");
	qmlRegisterType<bb::multimedia::ShuffleMode>("bb.cascades",1,3,"ShuffleMode");
	qmlRegisterType<bb::multimedia::SystemSound>("bb.cascades",1,3,"SystemSound");
	qmlRegisterType<bb::multimedia::VideoOutput>("bb.cascades",1,3,"VideoOutput");
	qmlRegisterType<bb::network::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::network::PushCommand>("bb.cascades",1,3,"PushCommand");
	qmlRegisterType<bb::network::PushErrorCode>("bb.cascades",1,3,"PushErrorCode");
	qmlRegisterType<bb::network::PushPayload>("bb.cascades",1,3,"PushPayload");
	qmlRegisterType<bb::network::PushService>("bb.cascades",1,3,"PushService");
	qmlRegisterType<bb::network::PushStatus>("bb.cascades",1,3,"PushStatus");
	qmlRegisterType<bb::location::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::location::PositionErrorCode>("bb.cascades",1,3,"PositionErrorCode");
	qmlRegisterType<bb::cascades::A11yComponentActivationEvent>("bb.cascades",1,3,"A11yComponentActivationEvent");
	qmlRegisterType<bb::cascades::A11yComponentActivationType>("bb.cascades",1,3,"A11yComponentActivationType");
	qmlRegisterType<bb::cascades::A11yMode>("bb.cascades",1,3,"A11yMode");
	qmlRegisterType<bb::cascades::A11yRole>("bb.cascades",1,3,"A11yRole");
	qmlRegisterType<bb::cascades::A11yState>("bb.cascades",1,3,"A11yState");
	qmlRegisterType<bb::cascades::A11yStateHolder>("bb.cascades",1,3,"A11yStateHolder");
	qmlRegisterType<bb::cascades::A11yValueAdjustmentType>("bb.cascades",1,3,"A11yValueAdjustmentType");
	qmlRegisterType<bb::cascades::AbsoluteLayout>("bb.cascades",1,3,"AbsoluteLayout");
	qmlRegisterType<bb::cascades::AbsoluteLayoutProperties>("bb.cascades",1,3,"AbsoluteLayoutProperties");
	qmlRegisterType<bb::cascades::AbstractA11yObject>("bb.cascades",1,3,"AbstractA11yObject");
	qmlRegisterType<bb::cascades::AbstractA11ySpecialization>("bb.cascades",1,3,"AbstractA11ySpecialization");
	qmlRegisterType<bb::cascades::AbstractActionItem>("bb.cascades",1,3,"AbstractActionItem");
	qmlRegisterType<bb::cascades::AbstractAnimation>("bb.cascades",1,3,"AbstractAnimation");
	qmlRegisterType<bb::cascades::AbstractButton>("bb.cascades",1,3,"AbstractButton");
	qmlRegisterType<bb::cascades::AbstractContentSecurity>("bb.cascades",1,3,"AbstractContentSecurity");
	qmlRegisterType<bb::cascades::AbstractCover>("bb.cascades",1,3,"AbstractCover");
	qmlRegisterType<bb::cascades::AbstractDialog>("bb.cascades",1,3,"AbstractDialog");
	qmlRegisterType<bb::cascades::AbstractEventHandler>("bb.cascades",1,3,"AbstractEventHandler");
	qmlRegisterType<bb::cascades::AbstractGestureEvent>("bb.cascades",1,3,"AbstractGestureEvent");
	qmlRegisterType<bb::cascades::AbstractPane>("bb.cascades",1,3,"AbstractPane");
	qmlRegisterType<bb::cascades::AbstractShortcut>("bb.cascades",1,3,"AbstractShortcut");
	qmlRegisterType<bb::cascades::AbstractTextControl>("bb.cascades",1,3,"AbstractTextControl");
	qmlRegisterType<bb::cascades::AbstractTextFilter>("bb.cascades",1,3,"AbstractTextFilter");
	qmlRegisterType<bb::cascades::AbstractTextStyleRule>("bb.cascades",1,3,"AbstractTextStyleRule");
	qmlRegisterType<bb::cascades::AbstractTitleBarKindProperties>("bb.cascades",1,3,"AbstractTitleBarKindProperties");
	qmlRegisterType<bb::cascades::AbstractToggleButton>("bb.cascades",1,3,"AbstractToggleButton");
	qmlRegisterType<bb::cascades::AbstractTransition>("bb.cascades",1,3,"AbstractTransition");
	qmlRegisterType<bb::cascades::ActionBarAutoHideBehavior>("bb.cascades",1,3,"ActionBarAutoHideBehavior");
	qmlRegisterType<bb::cascades::ActionBarFollowKeyboardPolicy>("bb.cascades",1,3,"ActionBarFollowKeyboardPolicy");
	qmlRegisterType<bb::cascades::ActionBarPlacement>("bb.cascades",1,3,"ActionBarPlacement");
	qmlRegisterType<bb::cascades::ActionItem>("bb.cascades",1,3,"ActionItem");
	qmlRegisterType<bb::cascades::ActionMenuVisualState>("bb.cascades",1,3,"ActionMenuVisualState");
	qmlRegisterType<bb::cascades::ActionSet>("bb.cascades",1,3,"ActionSet");
	qmlRegisterType<bb::cascades::ActiveTextHandler>("bb.cascades",1,3,"ActiveTextHandler");
	qmlRegisterType<bb::cascades::ActiveTextTriggeredEvent>("bb.cascades",1,3,"ActiveTextTriggeredEvent");
	qmlRegisterType<bb::cascades::ActivityIndicator>("bb.cascades",1,3,"ActivityIndicator");
	qmlRegisterType<bb::cascades::AdaptableExtensionPlugin>("bb.cascades",1,3,"AdaptableExtensionPlugin");
	qmlRegisterType<bb::cascades::AnimationRepeatCount>("bb.cascades",1,3,"AnimationRepeatCount");
	qmlRegisterType<bb::cascades::AnimationState>("bb.cascades",1,3,"AnimationState");
	qmlRegisterType<bb::cascades::Application>("bb.cascades",1,3,"Application");
	qmlRegisterType<bb::cascades::ApplicationViewCover>("bb.cascades",1,3,"ApplicationViewCover");
	qmlRegisterType<bb::cascades::ArrayDataModel>("bb.cascades",1,3,"ArrayDataModel");
	qmlRegisterType<bb::cascades::BaseObject>("bb.cascades",1,3,"BaseObject");
	qmlRegisterType<bb::cascades::Button>("bb.cascades",1,3,"Button");
	qmlRegisterType<bb::cascades::CheckBox>("bb.cascades",1,3,"CheckBox");
	qmlRegisterType<bb::cascades::ChromeVisibility>("bb.cascades",1,3,"ChromeVisibility");
	qmlRegisterType<bb::cascades::CollapseMode>("bb.cascades",1,3,"CollapseMode");
	qmlRegisterType<bb::cascades::Color>("bb.cascades",1,3,"Color");
	qmlRegisterType<bb::cascades::ColorPaint>("bb.cascades",1,3,"ColorPaint");
	qmlRegisterType<bb::cascades::ColorTheme>("bb.cascades",1,3,"ColorTheme");
	qmlRegisterType<bb::cascades::ComponentA11ySpecialization>("bb.cascades",1,3,"ComponentA11ySpecialization");
	qmlRegisterType<bb::cascades::Container>("bb.cascades",1,3,"Container");
	qmlRegisterType<bb::cascades::ContentSecurity>("bb.cascades",1,3,"ContentSecurity");
	qmlRegisterType<bb::cascades::ContextMenuHandler>("bb.cascades",1,3,"ContextMenuHandler");
	qmlRegisterType<bb::cascades::ContextMenuShowingEvent>("bb.cascades",1,3,"ContextMenuShowingEvent");
	qmlRegisterType<bb::cascades::ContextMenuVisualState>("bb.cascades",1,3,"ContextMenuVisualState");
	qmlRegisterType<bb::cascades::Control>("bb.cascades",1,3,"Control");
	qmlRegisterType<bb::cascades::ControlAppearance>("bb.cascades",1,3,"ControlAppearance");
	qmlRegisterType<bb::cascades::CoverDetailLevel>("bb.cascades",1,3,"CoverDetailLevel");
	qmlRegisterType<bb::cascades::CustomA11yObject>("bb.cascades",1,3,"CustomA11yObject");
	qmlRegisterType<bb::cascades::CustomControl>("bb.cascades",1,3,"CustomControl");
	qmlRegisterType<bb::cascades::CustomListItem>("bb.cascades",1,3,"CustomListItem");
	qmlRegisterType<bb::cascades::DataModel>("bb.cascades",1,3,"DataModel");
	qmlRegisterType<bb::cascades::DataModelChangeType>("bb.cascades",1,3,"DataModelChangeType");
	qmlRegisterType<bb::cascades::DateTimePicker>("bb.cascades",1,3,"DateTimePicker");
	qmlRegisterType<bb::cascades::DateTimePickerMode>("bb.cascades",1,3,"DateTimePickerMode");
	qmlRegisterType<bb::cascades::DeleteActionItem>("bb.cascades",1,3,"DeleteActionItem");
	qmlRegisterType<bb::cascades::DevelopmentSupport>("bb.cascades",1,3,"DevelopmentSupport");
	qmlRegisterType<bb::cascades::Dialog>("bb.cascades",1,3,"Dialog");
	qmlRegisterType<bb::cascades::DisplayDirection>("bb.cascades",1,3,"DisplayDirection");
	qmlRegisterType<bb::cascades::Divider>("bb.cascades",1,3,"Divider");
	qmlRegisterType<bb::cascades::DockLayout>("bb.cascades",1,3,"DockLayout");
	qmlRegisterType<bb::cascades::DoubleTapEvent>("bb.cascades",1,3,"DoubleTapEvent");
	qmlRegisterType<bb::cascades::DoubleTapHandler>("bb.cascades",1,3,"DoubleTapHandler");
	qmlRegisterType<bb::cascades::DropDown>("bb.cascades",1,3,"DropDown");
	qmlRegisterType<bb::cascades::EasingCurve>("bb.cascades",1,3,"EasingCurve");
	qmlRegisterType<bb::cascades::Event>("bb.cascades",1,3,"Event");
	qmlRegisterType<bb::cascades::EventReaction>("bb.cascades",1,3,"EventReaction");
	qmlRegisterType<bb::cascades::ExpandableView>("bb.cascades",1,3,"ExpandableView");
	qmlRegisterType<bb::cascades::ExpandMode>("bb.cascades",1,3,"ExpandMode");
	qmlRegisterType<bb::cascades::FadeTransition>("bb.cascades",1,3,"FadeTransition");
	qmlRegisterType<bb::cascades::FlickMode>("bb.cascades",1,3,"FlickMode");
	qmlRegisterType<bb::cascades::FlowListLayout>("bb.cascades",1,3,"FlowListLayout");
	qmlRegisterType<bb::cascades::FlowListLayoutProperties>("bb.cascades",1,3,"FlowListLayoutProperties");
	qmlRegisterType<bb::cascades::FocusAutoShow>("bb.cascades",1,3,"FocusAutoShow");
	qmlRegisterType<bb::cascades::FocusPolicy>("bb.cascades",1,3,"FocusPolicy");
	qmlRegisterType<bb::cascades::FocusRetentionPolicy>("bb.cascades",1,3,"FocusRetentionPolicy");
	qmlRegisterType<bb::cascades::FontFaceRule>("bb.cascades",1,3,"FontFaceRule");
	qmlRegisterType<bb::cascades::FontSize>("bb.cascades",1,3,"FontSize");
	qmlRegisterType<bb::cascades::FontStyle>("bb.cascades",1,3,"FontStyle");
	qmlRegisterType<bb::cascades::FontWeight>("bb.cascades",1,3,"FontWeight");
	qmlRegisterType<bb::cascades::ForeignWindowControl>("bb.cascades",1,3,"ForeignWindowControl");
	qmlRegisterType<bb::cascades::FreeFormTitleBarKindProperties>("bb.cascades",1,3,"FreeFormTitleBarKindProperties");
	qmlRegisterType<bb::cascades::GestureHandler>("bb.cascades",1,3,"GestureHandler");
	qmlRegisterType<bb::cascades::GridLayout>("bb.cascades",1,3,"GridLayout");
	qmlRegisterType<bb::cascades::GridListLayout>("bb.cascades",1,3,"GridListLayout");
	qmlRegisterType<bb::cascades::GroupAnimation>("bb.cascades",1,3,"GroupAnimation");
	qmlRegisterType<bb::cascades::GroupDataModel>("bb.cascades",1,3,"GroupDataModel");
	qmlRegisterType<bb::cascades::Header>("bb.cascades",1,3,"Header");
	qmlRegisterType<bb::cascades::HeaderMode>("bb.cascades",1,3,"HeaderMode");
	qmlRegisterType<bb::cascades::HelpActionItem>("bb.cascades",1,3,"HelpActionItem");
	qmlRegisterType<bb::cascades::HighlightAppearance>("bb.cascades",1,3,"HighlightAppearance");
	qmlRegisterType<bb::cascades::HorizontalAlignment>("bb.cascades",1,3,"HorizontalAlignment");
	qmlRegisterType<bb::cascades::Image>("bb.cascades",1,3,"Image");
	qmlRegisterType<bb::cascades::ImageAnimator>("bb.cascades",1,3,"ImageAnimator");
	qmlRegisterType<bb::cascades::ImageAnimatorAnchor>("bb.cascades",1,3,"ImageAnimatorAnchor");
	qmlRegisterType<bb::cascades::ImageAnimatorSeekMode>("bb.cascades",1,3,"ImageAnimatorSeekMode");
	qmlRegisterType<bb::cascades::ImageButton>("bb.cascades",1,3,"ImageButton");
	qmlRegisterType<bb::cascades::ImagePaint>("bb.cascades",1,3,"ImagePaint");
	qmlRegisterType<bb::cascades::ImageToggleButton>("bb.cascades",1,3,"ImageToggleButton");
	qmlRegisterType<bb::cascades::ImageTracker>("bb.cascades",1,3,"ImageTracker");
	qmlRegisterType<bb::cascades::ImageView>("bb.cascades",1,3,"ImageView");
	qmlRegisterType<bb::cascades::ImageViewLoadEffect>("bb.cascades",1,3,"ImageViewLoadEffect");
	qmlRegisterType<bb::cascades::ImplicitAnimationController>("bb.cascades",1,3,"ImplicitAnimationController");
	qmlRegisterType<bb::cascades::InputRouteProperties>("bb.cascades",1,3,"InputRouteProperties");
	qmlRegisterType<bb::cascades::Invocation>("bb.cascades",1,3,"Invocation");
	qmlRegisterType<bb::cascades::InvokeActionItem>("bb.cascades",1,3,"InvokeActionItem");
	qmlRegisterType<bb::cascades::InvokeHandler>("bb.cascades",1,3,"InvokeHandler");
	qmlRegisterType<bb::cascades::InvokeQuery>("bb.cascades",1,3,"InvokeQuery");
	qmlRegisterType<bb::cascades::InvokeState>("bb.cascades",1,3,"InvokeState");
	qmlRegisterType<bb::cascades::ItemGrouping>("bb.cascades",1,3,"ItemGrouping");
	qmlRegisterType<bb::cascades::JavaScriptWorld>("bb.cascades",1,3,"JavaScriptWorld");
	qmlRegisterType<bb::cascades::KeyEvent>("bb.cascades",1,3,"KeyEvent");
	qmlRegisterType<bb::cascades::KeyLayout>("bb.cascades",1,3,"KeyLayout");
	qmlRegisterType<bb::cascades::KeyListener>("bb.cascades",1,3,"KeyListener");
	qmlRegisterType<bb::cascades::KeyModifier>("bb.cascades",1,3,"KeyModifier");
	qmlRegisterType<bb::cascades::Label>("bb.cascades",1,3,"Label");
	qmlRegisterType<bb::cascades::LabelAutoSizeProperties>("bb.cascades",1,3,"LabelAutoSizeProperties");
	qmlRegisterType<bb::cascades::LabelTextFitMode>("bb.cascades",1,3,"LabelTextFitMode");
	qmlRegisterType<bb::cascades::LabelTextFitProperties>("bb.cascades",1,3,"LabelTextFitProperties");
	qmlRegisterType<bb::cascades::Layout>("bb.cascades",1,3,"Layout");
	qmlRegisterType<bb::cascades::LayoutOrientation>("bb.cascades",1,3,"LayoutOrientation");
	qmlRegisterType<bb::cascades::LayoutProperties>("bb.cascades",1,3,"LayoutProperties");
	qmlRegisterType<bb::cascades::LayoutUpdateHandler>("bb.cascades",1,3,"LayoutUpdateHandler");
	qmlRegisterType<bb::cascades::ListHeaderMode>("bb.cascades",1,3,"ListHeaderMode");
	qmlRegisterType<bb::cascades::ListItemListener>("bb.cascades",1,3,"ListItemListener");
	qmlRegisterType<bb::cascades::ListItemProvider>("bb.cascades",1,3,"ListItemProvider");
	qmlRegisterType<bb::cascades::ListItemTypeMapper>("bb.cascades",1,3,"ListItemTypeMapper");
	qmlRegisterType<bb::cascades::ListLayout>("bb.cascades",1,3,"ListLayout");
	qmlRegisterType<bb::cascades::ListRearrangeHandler>("bb.cascades",1,3,"ListRearrangeHandler");
	qmlRegisterType<bb::cascades::ListScrollStateHandler>("bb.cascades",1,3,"ListScrollStateHandler");
	qmlRegisterType<bb::cascades::ListView>("bb.cascades",1,3,"ListView");
	qmlRegisterType<bb::cascades::ListViewStickToEdgePolicy>("bb.cascades",1,3,"ListViewStickToEdgePolicy");
	qmlRegisterType<bb::cascades::LocaleHandler>("bb.cascades",1,3,"LocaleHandler");
	qmlRegisterType<bb::cascades::LongPressEvent>("bb.cascades",1,3,"LongPressEvent");
	qmlRegisterType<bb::cascades::LongPressHandler>("bb.cascades",1,3,"LongPressHandler");
	qmlRegisterType<bb::cascades::Menu>("bb.cascades",1,3,"Menu");
	qmlRegisterType<bb::cascades::MultiCover>("bb.cascades",1,3,"MultiCover");
	qmlRegisterType<bb::cascades::MultiSelectActionItem>("bb.cascades",1,3,"MultiSelectActionItem");
	qmlRegisterType<bb::cascades::MultiSelectHandler>("bb.cascades",1,3,"MultiSelectHandler");
	qmlRegisterType<bb::cascades::NavigationPane>("bb.cascades",1,3,"NavigationPane");
	qmlRegisterType<bb::cascades::NavigationPaneProperties>("bb.cascades",1,3,"NavigationPaneProperties");
	qmlRegisterType<bb::cascades::Option>("bb.cascades",1,3,"Option");
	qmlRegisterType<bb::cascades::OrientationSupport>("bb.cascades",1,3,"OrientationSupport");
	qmlRegisterType<bb::cascades::OverlapTouchPolicy>("bb.cascades",1,3,"OverlapTouchPolicy");
	qmlRegisterType<bb::cascades::OverScrollEffectMode>("bb.cascades",1,3,"OverScrollEffectMode");
	qmlRegisterType<bb::cascades::Page>("bb.cascades",1,3,"Page");
	qmlRegisterType<bb::cascades::PageLayoutUpdateHandler>("bb.cascades",1,3,"PageLayoutUpdateHandler");
	qmlRegisterType<bb::cascades::PageResizeBehavior>("bb.cascades",1,3,"PageResizeBehavior");
	qmlRegisterType<bb::cascades::Paint>("bb.cascades",1,3,"Paint");
	qmlRegisterType<bb::cascades::PaneProperties>("bb.cascades",1,3,"PaneProperties");
	qmlRegisterType<bb::cascades::ParallelAnimation>("bb.cascades",1,3,"ParallelAnimation");
	qmlRegisterType<bb::cascades::Picker>("bb.cascades",1,3,"Picker");
	qmlRegisterType<bb::cascades::PickerKind>("bb.cascades",1,3,"PickerKind");
	qmlRegisterType<bb::cascades::PickerProvider>("bb.cascades",1,3,"PickerProvider");
	qmlRegisterType<bb::cascades::PinchEvent>("bb.cascades",1,3,"PinchEvent");
	qmlRegisterType<bb::cascades::PinchHandler>("bb.cascades",1,3,"PinchHandler");
	qmlRegisterType<bb::cascades::ProgressIndicator>("bb.cascades",1,3,"ProgressIndicator");
	qmlRegisterType<bb::cascades::ProgressIndicatorState>("bb.cascades",1,3,"ProgressIndicatorState");
	qmlRegisterType<bb::cascades::PropagationPhase>("bb.cascades",1,3,"PropagationPhase");
	qmlRegisterType<bb::cascades::QListDataModel>("bb.cascades",1,3,"QListDataModel");
	qmlRegisterType<bb::cascades::QmlDocument>("bb.cascades",1,3,"QmlDocument");
	qmlRegisterType<bb::cascades::RadioGroup>("bb.cascades",1,3,"RadioGroup");
	qmlRegisterType<bb::cascades::RenderFence>("bb.cascades",1,3,"RenderFence");
	qmlRegisterType<bb::cascades::RenderFenceState>("bb.cascades",1,3,"RenderFenceState");
	qmlRegisterType<bb::cascades::RepeatPattern>("bb.cascades",1,3,"RepeatPattern");
	qmlRegisterType<bb::cascades::Resource>("bb.cascades",1,3,"Resource");
	qmlRegisterType<bb::cascades::ResourceState>("bb.cascades",1,3,"ResourceState");
	qmlRegisterType<bb::cascades::ResourceTracker>("bb.cascades",1,3,"ResourceTracker");
	qmlRegisterType<bb::cascades::RotateTransition>("bb.cascades",1,3,"RotateTransition");
	qmlRegisterType<bb::cascades::ScaleTransition>("bb.cascades",1,3,"ScaleTransition");
	qmlRegisterType<bb::cascades::ScalingMethod>("bb.cascades",1,3,"ScalingMethod");
	qmlRegisterType<bb::cascades::SceneCover>("bb.cascades",1,3,"SceneCover");
	qmlRegisterType<bb::cascades::ScreenIdleMode>("bb.cascades",1,3,"ScreenIdleMode");
	qmlRegisterType<bb::cascades::ScrollAnimation>("bb.cascades",1,3,"ScrollAnimation");
	qmlRegisterType<bb::cascades::ScrollIndicatorMode>("bb.cascades",1,3,"ScrollIndicatorMode");
	qmlRegisterType<bb::cascades::ScrollMode>("bb.cascades",1,3,"ScrollMode");
	qmlRegisterType<bb::cascades::ScrollPosition>("bb.cascades",1,3,"ScrollPosition");
	qmlRegisterType<bb::cascades::ScrollRailsPolicy>("bb.cascades",1,3,"ScrollRailsPolicy");
	qmlRegisterType<bb::cascades::ScrollRole>("bb.cascades",1,3,"ScrollRole");
	qmlRegisterType<bb::cascades::ScrollView>("bb.cascades",1,3,"ScrollView");
	qmlRegisterType<bb::cascades::ScrollViewProperties>("bb.cascades",1,3,"ScrollViewProperties");
	qmlRegisterType<bb::cascades::SegmentedControl>("bb.cascades",1,3,"SegmentedControl");
	qmlRegisterType<bb::cascades::SequentialAnimation>("bb.cascades",1,3,"SequentialAnimation");
	qmlRegisterType<bb::cascades::SettingsActionItem>("bb.cascades",1,3,"SettingsActionItem");
	qmlRegisterType<bb::cascades::Sheet>("bb.cascades",1,3,"Sheet");
	qmlRegisterType<bb::cascades::Shortcut>("bb.cascades",1,3,"Shortcut");
	qmlRegisterType<bb::cascades::ShortcutTriggerMode>("bb.cascades",1,3,"ShortcutTriggerMode");
	qmlRegisterType<bb::cascades::SidebarState>("bb.cascades",1,3,"SidebarState");
	qmlRegisterType<bb::cascades::SidebarVisualState>("bb.cascades",1,3,"SidebarVisualState");
	qmlRegisterType<bb::cascades::SingleLineTextFilter>("bb.cascades",1,3,"SingleLineTextFilter");
	qmlRegisterType<bb::cascades::Slider>("bb.cascades",1,3,"Slider");
	qmlRegisterType<bb::cascades::SnapMode>("bb.cascades",1,3,"SnapMode");
	qmlRegisterType<bb::cascades::Spacings>("bb.cascades",1,3,"Spacings");
	qmlRegisterType<bb::cascades::StackLayout>("bb.cascades",1,3,"StackLayout");
	qmlRegisterType<bb::cascades::StackLayoutProperties>("bb.cascades",1,3,"StackLayoutProperties");
	qmlRegisterType<bb::cascades::StackListLayout>("bb.cascades",1,3,"StackListLayout");
	qmlRegisterType<bb::cascades::StandardListItem>("bb.cascades",1,3,"StandardListItem");
	qmlRegisterType<bb::cascades::StandardPickerItem>("bb.cascades",1,3,"StandardPickerItem");
	qmlRegisterType<bb::cascades::StockCurve>("bb.cascades",1,3,"StockCurve");
	qmlRegisterType<bb::cascades::SubmitKey>("bb.cascades",1,3,"SubmitKey");
	qmlRegisterType<bb::cascades::SubmitKeyFocusBehavior>("bb.cascades",1,3,"SubmitKeyFocusBehavior");
	qmlRegisterType<bb::cascades::SupportedDisplayOrientation>("bb.cascades",1,3,"SupportedDisplayOrientation");
	qmlRegisterType<bb::cascades::SystemDefaults>("bb.cascades",1,3,"SystemDefaults");
	qmlRegisterType<bb::cascades::SystemShortcut>("bb.cascades",1,3,"SystemShortcut");
	qmlRegisterType<bb::cascades::SystemShortcuts>("bb.cascades",1,3,"SystemShortcuts");
	qmlRegisterType<bb::cascades::Tab>("bb.cascades",1,3,"Tab");
	qmlRegisterType<bb::cascades::TabbedPane>("bb.cascades",1,3,"TabbedPane");
	qmlRegisterType<bb::cascades::TapEvent>("bb.cascades",1,3,"TapEvent");
	qmlRegisterType<bb::cascades::TapHandler>("bb.cascades",1,3,"TapHandler");
	qmlRegisterType<bb::cascades::TextAlign>("bb.cascades",1,3,"TextAlign");
	qmlRegisterType<bb::cascades::TextArea>("bb.cascades",1,3,"TextArea");
	qmlRegisterType<bb::cascades::TextAreaAutoSizeProperties>("bb.cascades",1,3,"TextAreaAutoSizeProperties");
	qmlRegisterType<bb::cascades::TextAreaInputMode>("bb.cascades",1,3,"TextAreaInputMode");
	qmlRegisterType<bb::cascades::TextAreaScrollMode>("bb.cascades",1,3,"TextAreaScrollMode");
	qmlRegisterType<bb::cascades::TextAreaTextFitProperties>("bb.cascades",1,3,"TextAreaTextFitProperties");
	qmlRegisterType<bb::cascades::TextAutoFit>("bb.cascades",1,3,"TextAutoFit");
	qmlRegisterType<bb::cascades::TextContentFlag>("bb.cascades",1,3,"TextContentFlag");
	qmlRegisterType<bb::cascades::TextContentProperties>("bb.cascades",1,3,"TextContentProperties");
	qmlRegisterType<bb::cascades::TextEditor>("bb.cascades",1,3,"TextEditor");
	qmlRegisterType<bb::cascades::TextField>("bb.cascades",1,3,"TextField");
	qmlRegisterType<bb::cascades::TextFieldInputMode>("bb.cascades",1,3,"TextFieldInputMode");
	qmlRegisterType<bb::cascades::TextFieldTextFitProperties>("bb.cascades",1,3,"TextFieldTextFitProperties");
	qmlRegisterType<bb::cascades::TextFieldTitleBarKindProperties>("bb.cascades",1,3,"TextFieldTitleBarKindProperties");
	qmlRegisterType<bb::cascades::TextFitProperties>("bb.cascades",1,3,"TextFitProperties");
	qmlRegisterType<bb::cascades::TextFormat>("bb.cascades",1,3,"TextFormat");
	qmlRegisterType<bb::cascades::TextInputActionItem>("bb.cascades",1,3,"TextInputActionItem");
	qmlRegisterType<bb::cascades::TextInputFlag>("bb.cascades",1,3,"TextInputFlag");
	qmlRegisterType<bb::cascades::TextInputMasking>("bb.cascades",1,3,"TextInputMasking");
	qmlRegisterType<bb::cascades::TextInputProperties>("bb.cascades",1,3,"TextInputProperties");
	qmlRegisterType<bb::cascades::TextStyle>("bb.cascades",1,3,"TextStyle");
	qmlRegisterType<bb::cascades::TextStyleDefinition>("bb.cascades",1,3,"TextStyleDefinition");
	qmlRegisterType<bb::cascades::Theme>("bb.cascades",1,3,"Theme");
	qmlRegisterType<bb::cascades::ThemeSupport>("bb.cascades",1,3,"ThemeSupport");
	qmlRegisterType<bb::cascades::TitleBar>("bb.cascades",1,3,"TitleBar");
	qmlRegisterType<bb::cascades::TitleBarAppearance>("bb.cascades",1,3,"TitleBarAppearance");
	qmlRegisterType<bb::cascades::TitleBarExpandableArea>("bb.cascades",1,3,"TitleBarExpandableArea");
	qmlRegisterType<bb::cascades::TitleBarExpandableAreaIndicatorVisibility>("bb.cascades",1,3,"TitleBarExpandableAreaIndicatorVisibility");
	qmlRegisterType<bb::cascades::TitleBarExpandableAreaToggleArea>("bb.cascades",1,3,"TitleBarExpandableAreaToggleArea");
	qmlRegisterType<bb::cascades::TitleBarKind>("bb.cascades",1,3,"TitleBarKind");
	qmlRegisterType<bb::cascades::TitleBarScrollBehavior>("bb.cascades",1,3,"TitleBarScrollBehavior");
	qmlRegisterType<bb::cascades::ToggleButton>("bb.cascades",1,3,"ToggleButton");
	qmlRegisterType<bb::cascades::TouchBehavior>("bb.cascades",1,3,"TouchBehavior");
	qmlRegisterType<bb::cascades::TouchEnterEvent>("bb.cascades",1,3,"TouchEnterEvent");
	qmlRegisterType<bb::cascades::TouchEvent>("bb.cascades",1,3,"TouchEvent");
	qmlRegisterType<bb::cascades::TouchExitEvent>("bb.cascades",1,3,"TouchExitEvent");
	qmlRegisterType<bb::cascades::TouchKeyboardEvent>("bb.cascades",1,3,"TouchKeyboardEvent");
	qmlRegisterType<bb::cascades::TouchKeyboardHandler>("bb.cascades",1,3,"TouchKeyboardHandler");
	qmlRegisterType<bb::cascades::TouchPropagationMode>("bb.cascades",1,3,"TouchPropagationMode");
	qmlRegisterType<bb::cascades::TouchReaction>("bb.cascades",1,3,"TouchReaction");
	qmlRegisterType<bb::cascades::TouchResponse>("bb.cascades",1,3,"TouchResponse");
	qmlRegisterType<bb::cascades::TouchType>("bb.cascades",1,3,"TouchType");
	qmlRegisterType<bb::cascades::TranslateTransition>("bb.cascades",1,3,"TranslateTransition");
	qmlRegisterType<bb::cascades::UIConfig>("bb.cascades",1,3,"UIConfig");
	qmlRegisterType<bb::cascades::UIObject>("bb.cascades",1,3,"UIObject");
	qmlRegisterType<bb::cascades::UIOrientation>("bb.cascades",1,3,"UIOrientation");
	qmlRegisterType<bb::cascades::UIPalette>("bb.cascades",1,3,"UIPalette");
	qmlRegisterType<bb::cascades::ValidationMode>("bb.cascades",1,3,"ValidationMode");
	qmlRegisterType<bb::cascades::ValidationState>("bb.cascades",1,3,"ValidationState");
	qmlRegisterType<bb::cascades::Validator>("bb.cascades",1,3,"Validator");
	qmlRegisterType<bb::cascades::ValueA11ySpecialization>("bb.cascades",1,3,"ValueA11ySpecialization");
	qmlRegisterType<bb::cascades::VerticalAlignment>("bb.cascades",1,3,"VerticalAlignment");
	qmlRegisterType<bb::cascades::VisualNode>("bb.cascades",1,3,"VisualNode");
	qmlRegisterType<bb::cascades::VisualStyle>("bb.cascades",1,3,"VisualStyle");
	qmlRegisterType<bb::cascades::WebCookieJar>("bb.cascades",1,3,"WebCookieJar");
	qmlRegisterType<bb::cascades::WebDownloadRequest>("bb.cascades",1,3,"WebDownloadRequest");
	qmlRegisterType<bb::cascades::WebFindFlag>("bb.cascades",1,3,"WebFindFlag");
	qmlRegisterType<bb::cascades::WebInputProperties>("bb.cascades",1,3,"WebInputProperties");
	qmlRegisterType<bb::cascades::WebLoadRequest>("bb.cascades",1,3,"WebLoadRequest");
	qmlRegisterType<bb::cascades::WebLoadStatus>("bb.cascades",1,3,"WebLoadStatus");
	qmlRegisterType<bb::cascades::WebNavigationRequest>("bb.cascades",1,3,"WebNavigationRequest");
	qmlRegisterType<bb::cascades::WebNavigationRequestAction>("bb.cascades",1,3,"WebNavigationRequestAction");
	qmlRegisterType<bb::cascades::WebNavigationType>("bb.cascades",1,3,"WebNavigationType");
	qmlRegisterType<bb::cascades::WebNewViewRequest>("bb.cascades",1,3,"WebNewViewRequest");
	qmlRegisterType<bb::cascades::WebPage>("bb.cascades",1,3,"WebPage");
	qmlRegisterType<bb::cascades::WebPageCompositor>("bb.cascades",1,3,"WebPageCompositor");
	qmlRegisterType<bb::cascades::WebPageEventHandler>("bb.cascades",1,3,"WebPageEventHandler");
	qmlRegisterType<bb::cascades::WebPageTexture>("bb.cascades",1,3,"WebPageTexture");
	qmlRegisterType<bb::cascades::WebResourceRequest>("bb.cascades",1,3,"WebResourceRequest");
	qmlRegisterType<bb::cascades::WebResourceRequestFilter>("bb.cascades",1,3,"WebResourceRequestFilter");
	qmlRegisterType<bb::cascades::WebSettings>("bb.cascades",1,3,"WebSettings");
	qmlRegisterType<bb::cascades::WebStorage>("bb.cascades",1,3,"WebStorage");
	qmlRegisterType<bb::cascades::WebView>("bb.cascades",1,3,"WebView");
	qmlRegisterType<bb::cascades::Window>("bb.cascades",1,3,"Window");
	qmlRegisterType<bb::cascades::WindowProperty>("bb.cascades",1,3,"WindowProperty");
	qmlRegisterType<bb::cascades::WindowState>("bb.cascades",1,3,"WindowState");
	qmlRegisterType<bb::cascades::XmlDataModel>("bb.cascades",1,3,"XmlDataModel");
	qmlRegisterType<bb::cascades::BarcodeDetector>("bb.cascades",1,3,"BarcodeDetector");
	qmlRegisterType<bb::cascades::BarcodeDetectorVisuals>("bb.cascades",1,3,"BarcodeDetectorVisuals");
	qmlRegisterType<bb::cascades::BarcodeFormat>("bb.cascades",1,3,"BarcodeFormat");
	qmlRegisterType<bb::cascades::BarcodeGenerator>("bb.cascades",1,3,"BarcodeGenerator");
	qmlRegisterType<bb::cascades::BarcodeGeneratorError>("bb.cascades",1,3,"BarcodeGeneratorError");
	qmlRegisterType<bb::cascades::BarcodeGeneratorFormat>("bb.cascades",1,3,"BarcodeGeneratorFormat");
	qmlRegisterType<bb::cascades::BarcodeView>("bb.cascades",1,3,"BarcodeView");
	qmlRegisterType<bb::cascades::Camera>("bb.cascades",1,3,"Camera");
	qmlRegisterType<bb::cascades::CameraError>("bb.cascades",1,3,"CameraError");
	qmlRegisterType<bb::cascades::CameraFlashMode>("bb.cascades",1,3,"CameraFlashMode");
	qmlRegisterType<bb::cascades::CameraFocusMode>("bb.cascades",1,3,"CameraFocusMode");
	qmlRegisterType<bb::cascades::CameraFocusState>("bb.cascades",1,3,"CameraFocusState");
	qmlRegisterType<bb::cascades::CameraMode>("bb.cascades",1,3,"CameraMode");
	qmlRegisterType<bb::cascades::CameraSceneMode>("bb.cascades",1,3,"CameraSceneMode");
	qmlRegisterType<bb::cascades::CameraSettings>("bb.cascades",1,3,"CameraSettings");
	qmlRegisterType<bb::cascades::CameraShootingMode>("bb.cascades",1,3,"CameraShootingMode");
	qmlRegisterType<bb::cascades::CameraUnit>("bb.cascades",1,3,"CameraUnit");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::QrCodeErrorCorrection>("bb.cascades",1,3,"QrCodeErrorCorrection");
	qmlRegisterType<bb::cascades::QrCodeRenderResult>("bb.cascades",1,3,"QrCodeRenderResult");
	qmlRegisterType<bb::cascades::QrCodeView>("bb.cascades",1,3,"QrCodeView");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::PlacePicker>("bb.cascades",1,3,"PlacePicker");
	qmlRegisterType<bb::cascades::PlacePickerControl>("bb.cascades",1,3,"PlacePickerControl");
	qmlRegisterType<bb::cascades::SelectedPlace>("bb.cascades",1,3,"SelectedPlace");
	qmlRegisterType<bb::cascades::SelectedPlaceImageType>("bb.cascades",1,3,"SelectedPlaceImageType");
	qmlRegisterType<bb::cascades::Banner>("bb.cascades",1,3,"Banner");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::CalendarEventPicker>("bb.cascades",1,3,"CalendarEventPicker");
	qmlRegisterType<bb::cascades::CalendarEventPickerError>("bb.cascades",1,3,"CalendarEventPickerError");
	qmlRegisterType<bb::cascades::ContactPicker>("bb.cascades",1,3,"ContactPicker");
	qmlRegisterType<bb::cascades::ContactSelectionMode>("bb.cascades",1,3,"ContactSelectionMode");
	qmlRegisterType<bb::cascades::FilePicker>("bb.cascades",1,3,"FilePicker");
	qmlRegisterType<bb::cascades::FilePickerError>("bb.cascades",1,3,"FilePickerError");
	qmlRegisterType<bb::cascades::FilePickerMode>("bb.cascades",1,3,"FilePickerMode");
	qmlRegisterType<bb::cascades::FilePickerSortFlag>("bb.cascades",1,3,"FilePickerSortFlag");
	qmlRegisterType<bb::cascades::FilePickerSortOrder>("bb.cascades",1,3,"FilePickerSortOrder");
	qmlRegisterType<bb::cascades::FilePickerSourceRestriction>("bb.cascades",1,3,"FilePickerSourceRestriction");
	qmlRegisterType<bb::cascades::FilePickerViewMode>("bb.cascades",1,3,"FilePickerViewMode");
	qmlRegisterType<bb::cascades::FileType>("bb.cascades",1,3,"FileType");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::ICalendarConstants>("bb.cascades",1,3,"ICalendarConstants");
	qmlRegisterType<bb::cascades::SelectedContactAttribute>("bb.cascades",1,3,"SelectedContactAttribute");
	qmlRegisterType<bb::cascades::AsyncDataModel>("bb.cascades",1,3,"AsyncDataModel");
	qmlRegisterType<bb::cascades::AsyncHeaderDataModel>("bb.cascades",1,3,"AsyncHeaderDataModel");
	qmlRegisterType<bb::cascades::AsyncHierarchicalDataModel>("bb.cascades",1,3,"AsyncHierarchicalDataModel");
	qmlRegisterType<bb::cascades::DataItem>("bb.cascades",1,3,"DataItem");
	qmlRegisterType<bb::cascades::DataModelDecorator>("bb.cascades",1,3,"DataModelDecorator");
	qmlRegisterType<bb::cascades::DataQuery>("bb.cascades",1,3,"DataQuery");
	qmlRegisterType<bb::cascades::DataQueryDecorator>("bb.cascades",1,3,"DataQueryDecorator");
	qmlRegisterType<bb::cascades::DataRevision>("bb.cascades",1,3,"DataRevision");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::HeaderDataItem>("bb.cascades",1,3,"HeaderDataItem");
	qmlRegisterType<bb::cascades::HeaderDataQuery>("bb.cascades",1,3,"HeaderDataQuery");
	qmlRegisterType<bb::cascades::NumericRevision>("bb.cascades",1,3,"NumericRevision");
	qmlRegisterType<bb::cascades::QueryDataModel>("bb.cascades",1,3,"QueryDataModel");
	qmlRegisterType<bb::cascades::Revision>("bb.cascades",1,3,"Revision");
	qmlRegisterType<bb::cascades::SimpleQueryDataModel>("bb.cascades",1,3,"SimpleQueryDataModel");
	qmlRegisterType<bb::cascades::SqlDataQuery>("bb.cascades",1,3,"SqlDataQuery");
	qmlRegisterType<bb::cascades::SqlHeaderDataQuery>("bb.cascades",1,3,"SqlHeaderDataQuery");
	qmlRegisterType<bb::cascades::SqlQueryUtils>("bb.cascades",1,3,"SqlQueryUtils");
	qmlRegisterType<bb::cascades::BlankRenderEngine>("bb.cascades",1,3,"BlankRenderEngine");
	qmlRegisterType<bb::cascades::DataProvider>("bb.cascades",1,3,"DataProvider");
	qmlRegisterType<bb::cascades::GeographicsProvisioningBridge>("bb.cascades",1,3,"GeographicsProvisioningBridge");
	qmlRegisterType<bb::cascades::GeographicsProvisioningBridgeDataSource>("bb.cascades",1,3,"GeographicsProvisioningBridgeDataSource");
	qmlRegisterType<bb::cascades::GeographicsProvisioningBridgeResult>("bb.cascades",1,3,"GeographicsProvisioningBridgeResult");
	qmlRegisterType<bb::cascades::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::cascades::KMLConversion>("bb.cascades",1,3,"KMLConversion");
	qmlRegisterType<bb::cascades::MapCoverageChecker>("bb.cascades",1,3,"MapCoverageChecker");
	qmlRegisterType<bb::cascades::MapData>("bb.cascades",1,3,"MapData");
	qmlRegisterType<bb::cascades::MapFactory>("bb.cascades",1,3,"MapFactory");
	qmlRegisterType<bb::cascades::MapImageCacheManager>("bb.cascades",1,3,"MapImageCacheManager");
	qmlRegisterType<bb::cascades::MapImageGenerator>("bb.cascades",1,3,"MapImageGenerator");
	qmlRegisterType<bb::cascades::MapImageGeneratorError>("bb.cascades",1,3,"MapImageGeneratorError");
	qmlRegisterType<bb::cascades::MapLongPressToPinDrop>("bb.cascades",1,3,"MapLongPressToPinDrop");
	qmlRegisterType<bb::cascades::MapView>("bb.cascades",1,3,"MapView");
	qmlRegisterType<bb::cascades::RenderEngine>("bb.cascades",1,3,"RenderEngine");
	qmlRegisterType<bb::cascades::RenderEngineConfiguration>("bb.cascades",1,3,"RenderEngineConfiguration");
	qmlRegisterType<bb::cascades::RenderEngineInfo>("bb.cascades",1,3,"RenderEngineInfo");
	qmlRegisterType<bb::cascades::ViewProperties>("bb.cascades",1,3,"ViewProperties");
	qmlRegisterType<bb::pim::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::pim::Notebook>("bb.cascades",1,3,"Notebook");
	qmlRegisterType<bb::pim::NotebookConstraint>("bb.cascades",1,3,"NotebookConstraint");
	qmlRegisterType<bb::pim::NotebookDomain>("bb.cascades",1,3,"NotebookDomain");
	qmlRegisterType<bb::pim::NotebookEntry>("bb.cascades",1,3,"NotebookEntry");
	qmlRegisterType<bb::pim::NotebookEntryAttachment>("bb.cascades",1,3,"NotebookEntryAttachment");
	qmlRegisterType<bb::pim::NotebookEntryConstraint>("bb.cascades",1,3,"NotebookEntryConstraint");
	qmlRegisterType<bb::pim::NotebookEntryDescription>("bb.cascades",1,3,"NotebookEntryDescription");
	qmlRegisterType<bb::pim::NotebookEntryDetailLevel>("bb.cascades",1,3,"NotebookEntryDetailLevel");
	qmlRegisterType<bb::pim::NotebookEntryDueDate>("bb.cascades",1,3,"NotebookEntryDueDate");
	qmlRegisterType<bb::pim::NotebookEntryFilter>("bb.cascades",1,3,"NotebookEntryFilter");
	qmlRegisterType<bb::pim::NotebookEntryHeader>("bb.cascades",1,3,"NotebookEntryHeader");
	qmlRegisterType<bb::pim::NotebookEntryId>("bb.cascades",1,3,"NotebookEntryId");
	qmlRegisterType<bb::pim::NotebookEntrySearchField>("bb.cascades",1,3,"NotebookEntrySearchField");
	qmlRegisterType<bb::pim::NotebookEntrySortColumn>("bb.cascades",1,3,"NotebookEntrySortColumn");
	qmlRegisterType<bb::pim::NotebookEntrySortOrder>("bb.cascades",1,3,"NotebookEntrySortOrder");
	qmlRegisterType<bb::pim::NotebookEntryStatus>("bb.cascades",1,3,"NotebookEntryStatus");
	qmlRegisterType<bb::pim::NotebookFilter>("bb.cascades",1,3,"NotebookFilter");
	qmlRegisterType<bb::pim::NotebookId>("bb.cascades",1,3,"NotebookId");
	qmlRegisterType<bb::pim::NotebookService>("bb.cascades",1,3,"NotebookService");
	qmlRegisterType<bb::pim::NotebookServiceResult>("bb.cascades",1,3,"NotebookServiceResult");
	qmlRegisterType<bb::pim::Contact>("bb.cascades",1,3,"Contact");
	qmlRegisterType<bb::pim::ContactActivity>("bb.cascades",1,3,"ContactActivity");
	qmlRegisterType<bb::pim::ContactAttribute>("bb.cascades",1,3,"ContactAttribute");
	qmlRegisterType<bb::pim::ContactAttributeBuilder>("bb.cascades",1,3,"ContactAttributeBuilder");
	qmlRegisterType<bb::pim::ContactAutoCompleteSearchFilters>("bb.cascades",1,3,"ContactAutoCompleteSearchFilters");
	qmlRegisterType<bb::pim::ContactBuilder>("bb.cascades",1,3,"ContactBuilder");
	qmlRegisterType<bb::pim::ContactConsts>("bb.cascades",1,3,"ContactConsts");
	qmlRegisterType<bb::pim::ContactFavouriteAction>("bb.cascades",1,3,"ContactFavouriteAction");
	qmlRegisterType<bb::pim::ContactFavouriteActionBuilder>("bb.cascades",1,3,"ContactFavouriteActionBuilder");
	qmlRegisterType<bb::pim::ContactGroup>("bb.cascades",1,3,"ContactGroup");
	qmlRegisterType<bb::pim::ContactGroupBuilder>("bb.cascades",1,3,"ContactGroupBuilder");
	qmlRegisterType<bb::pim::ContactListFilters>("bb.cascades",1,3,"ContactListFilters");
	qmlRegisterType<bb::pim::ContactNews>("bb.cascades",1,3,"ContactNews");
	qmlRegisterType<bb::pim::ContactOnlineStatus>("bb.cascades",1,3,"ContactOnlineStatus");
	qmlRegisterType<bb::pim::ContactPhoto>("bb.cascades",1,3,"ContactPhoto");
	qmlRegisterType<bb::pim::ContactPhotoBuilder>("bb.cascades",1,3,"ContactPhotoBuilder");
	qmlRegisterType<bb::pim::ContactPostalAddress>("bb.cascades",1,3,"ContactPostalAddress");
	qmlRegisterType<bb::pim::ContactPostalAddressBuilder>("bb.cascades",1,3,"ContactPostalAddressBuilder");
	qmlRegisterType<bb::pim::ContactRemoteSearchFilters>("bb.cascades",1,3,"ContactRemoteSearchFilters");
	qmlRegisterType<bb::pim::ContactSearchFilters>("bb.cascades",1,3,"ContactSearchFilters");
	qmlRegisterType<bb::pim::ContactService>("bb.cascades",1,3,"ContactService");
	qmlRegisterType<bb::pim::AnalyticsParams>("bb.cascades",1,3,"AnalyticsParams");
	qmlRegisterType<bb::pim::AnalyticsStats>("bb.cascades",1,3,"AnalyticsStats");
	qmlRegisterType<bb::pim::Attendee>("bb.cascades",1,3,"Attendee");
	qmlRegisterType<bb::pim::AttendeeInEvents>("bb.cascades",1,3,"AttendeeInEvents");
	qmlRegisterType<bb::pim::AttendeeRole>("bb.cascades",1,3,"AttendeeRole");
	qmlRegisterType<bb::pim::AttendeeSearchParameters>("bb.cascades",1,3,"AttendeeSearchParameters");
	qmlRegisterType<bb::pim::AttendeeStatus>("bb.cascades",1,3,"AttendeeStatus");
	qmlRegisterType<bb::pim::BbmConference>("bb.cascades",1,3,"BbmConference");
	qmlRegisterType<bb::pim::BbmConferencePreferredData>("bb.cascades",1,3,"BbmConferencePreferredData");
	qmlRegisterType<bb::pim::BbmConferenceUserData>("bb.cascades",1,3,"BbmConferenceUserData");
	qmlRegisterType<bb::pim::BusyStatus>("bb.cascades",1,3,"BusyStatus");
	qmlRegisterType<bb::pim::CalendarEvent>("bb.cascades",1,3,"CalendarEvent");
	qmlRegisterType<bb::pim::CalendarFolder>("bb.cascades",1,3,"CalendarFolder");
	qmlRegisterType<bb::pim::CalendarService>("bb.cascades",1,3,"CalendarService");
	qmlRegisterType<bb::pim::CalendarSettings>("bb.cascades",1,3,"CalendarSettings");
	qmlRegisterType<bb::pim::CommonAttendee>("bb.cascades",1,3,"CommonAttendee");
	qmlRegisterType<bb::pim::CommonLocation>("bb.cascades",1,3,"CommonLocation");
	qmlRegisterType<bb::pim::DataTypes>("bb.cascades",1,3,"DataTypes");
	qmlRegisterType<bb::pim::DayOfWeek>("bb.cascades",1,3,"DayOfWeek");
	qmlRegisterType<bb::pim::DetailLevel>("bb.cascades",1,3,"DetailLevel");
	qmlRegisterType<bb::pim::EventKey>("bb.cascades",1,3,"EventKey");
	qmlRegisterType<bb::pim::EventRefresh>("bb.cascades",1,3,"EventRefresh");
	qmlRegisterType<bb::pim::EventSearchParameters>("bb.cascades",1,3,"EventSearchParameters");
	qmlRegisterType<bb::pim::FolderKey>("bb.cascades",1,3,"FolderKey");
	qmlRegisterType<bb::pim::Frequency>("bb.cascades",1,3,"Frequency");
	qmlRegisterType<bb::pim::ICalendarData>("bb.cascades",1,3,"ICalendarData");
	qmlRegisterType<bb::pim::ICalendarDataPath>("bb.cascades",1,3,"ICalendarDataPath");
	qmlRegisterType<bb::pim::ICalendarObjects>("bb.cascades",1,3,"ICalendarObjects");
	qmlRegisterType<bb::pim::ICalendarTodo>("bb.cascades",1,3,"ICalendarTodo");
	qmlRegisterType<bb::pim::ICalendarTodoPriority>("bb.cascades",1,3,"ICalendarTodoPriority");
	qmlRegisterType<bb::pim::ICalendarTodoStatus>("bb.cascades",1,3,"ICalendarTodoStatus");
	qmlRegisterType<bb::pim::MeetingHistory>("bb.cascades",1,3,"MeetingHistory");
	qmlRegisterType<bb::pim::Notification>("bb.cascades",1,3,"Notification");
	qmlRegisterType<bb::pim::OnlineConferenceData>("bb.cascades",1,3,"OnlineConferenceData");
	qmlRegisterType<bb::pim::Recurrence>("bb.cascades",1,3,"Recurrence");
	qmlRegisterType<bb::pim::Result>("bb.cascades",1,3,"Result");
	qmlRegisterType<bb::pim::Sensitivity>("bb.cascades",1,3,"Sensitivity");
	qmlRegisterType<bb::pim::SortField>("bb.cascades",1,3,"SortField");
	qmlRegisterType<bb::pim::WeekStartDay>("bb.cascades",1,3,"WeekStartDay");
	qmlRegisterType<bb::pim::Attachment>("bb.cascades",1,3,"Attachment");
	qmlRegisterType<bb::pim::AttachmentDownloadStatus>("bb.cascades",1,3,"AttachmentDownloadStatus");
	qmlRegisterType<bb::pim::AttachmentEncoding>("bb.cascades",1,3,"AttachmentEncoding");
	qmlRegisterType<bb::pim::AttachmentFilter>("bb.cascades",1,3,"AttachmentFilter");
	qmlRegisterType<bb::pim::Conversation>("bb.cascades",1,3,"Conversation");
	qmlRegisterType<bb::pim::ConversationBuilder>("bb.cascades",1,3,"ConversationBuilder");
	qmlRegisterType<bb::pim::ConversationStatus>("bb.cascades",1,3,"ConversationStatus");
	qmlRegisterType<bb::pim::ConversationUpdate>("bb.cascades",1,3,"ConversationUpdate");
	qmlRegisterType<bb::pim::FileAttachment>("bb.cascades",1,3,"FileAttachment");
	qmlRegisterType<bb::pim::Filter>("bb.cascades",1,3,"Filter");
	qmlRegisterType<bb::pim::Folder>("bb.cascades",1,3,"Folder");
	qmlRegisterType<bb::pim::FollowupFlag>("bb.cascades",1,3,"FollowupFlag");
	qmlRegisterType<bb::pim::Keys>("bb.cascades",1,3,"Keys");
	qmlRegisterType<bb::pim::MarkOrDeletePriorFilter>("bb.cascades",1,3,"MarkOrDeletePriorFilter");
	qmlRegisterType<bb::pim::MarkOrDeletePriorOp>("bb.cascades",1,3,"MarkOrDeletePriorOp");
	qmlRegisterType<bb::pim::Message>("bb.cascades",1,3,"Message");
	qmlRegisterType<bb::pim::MessageBody>("bb.cascades",1,3,"MessageBody");
	qmlRegisterType<bb::pim::MessageBuilder>("bb.cascades",1,3,"MessageBuilder");
	qmlRegisterType<bb::pim::MessageClass>("bb.cascades",1,3,"MessageClass");
	qmlRegisterType<bb::pim::MessageContact>("bb.cascades",1,3,"MessageContact");
	qmlRegisterType<bb::pim::MessageFilter>("bb.cascades",1,3,"MessageFilter");
	qmlRegisterType<bb::pim::MessageFlag>("bb.cascades",1,3,"MessageFlag");
	qmlRegisterType<bb::pim::MessageFolder>("bb.cascades",1,3,"MessageFolder");
	qmlRegisterType<bb::pim::MessagePriority>("bb.cascades",1,3,"MessagePriority");
	qmlRegisterType<bb::pim::MessageReplyState>("bb.cascades",1,3,"MessageReplyState");
	qmlRegisterType<bb::pim::MessageRights>("bb.cascades",1,3,"MessageRights");
	qmlRegisterType<bb::pim::MessageSearchFilter>("bb.cascades",1,3,"MessageSearchFilter");
	qmlRegisterType<bb::pim::MessageService>("bb.cascades",1,3,"MessageService");
	qmlRegisterType<bb::pim::MessageServiceFeature>("bb.cascades",1,3,"MessageServiceFeature");
	qmlRegisterType<bb::pim::MessageStatus>("bb.cascades",1,3,"MessageStatus");
	qmlRegisterType<bb::pim::MessageTransaction>("bb.cascades",1,3,"MessageTransaction");
	qmlRegisterType<bb::pim::MessageTransmissionStatus>("bb.cascades",1,3,"MessageTransmissionStatus");
	qmlRegisterType<bb::pim::MessageUpdate>("bb.cascades",1,3,"MessageUpdate");
	qmlRegisterType<bb::pim::MessageUpdateFilter>("bb.cascades",1,3,"MessageUpdateFilter");
	qmlRegisterType<bb::pim::MessageUpdateType>("bb.cascades",1,3,"MessageUpdateType");
	qmlRegisterType<bb::pim::MimeTypes>("bb.cascades",1,3,"MimeTypes");
	qmlRegisterType<bb::pim::OutOfOfficeSettings>("bb.cascades",1,3,"OutOfOfficeSettings");
	qmlRegisterType<bb::pim::SearchFilterCriteria>("bb.cascades",1,3,"SearchFilterCriteria");
	qmlRegisterType<bb::pim::SearchStatusCriteria>("bb.cascades",1,3,"SearchStatusCriteria");
	qmlRegisterType<bb::pim::SmsTransport>("bb.cascades",1,3,"SmsTransport");
	qmlRegisterType<bb::pim::SmsTransportRegistrationResult>("bb.cascades",1,3,"SmsTransportRegistrationResult");
	qmlRegisterType<bb::pim::SmsTransportSendResult>("bb.cascades",1,3,"SmsTransportSendResult");
	qmlRegisterType<bb::pim::StatusReport>("bb.cascades",1,3,"StatusReport");
	qmlRegisterType<bb::pim::StatusReportType>("bb.cascades",1,3,"StatusReportType");
	qmlRegisterType<bb::pim::ContactEntry>("bb.cascades",1,3,"ContactEntry");
	qmlRegisterType<bb::pim::CallAttribute>("bb.cascades",1,3,"CallAttribute");
	qmlRegisterType<bb::pim::CallEntry>("bb.cascades",1,3,"CallEntry");
	qmlRegisterType<bb::pim::CallEntryResult>("bb.cascades",1,3,"CallEntryResult");
	qmlRegisterType<bb::pim::CallHistoryError>("bb.cascades",1,3,"CallHistoryError");
	qmlRegisterType<bb::pim::CallHistoryFilter>("bb.cascades",1,3,"CallHistoryFilter");
	qmlRegisterType<bb::pim::CallHistoryParam>("bb.cascades",1,3,"CallHistoryParam");
	qmlRegisterType<bb::pim::CallHistoryService>("bb.cascades",1,3,"CallHistoryService");
	qmlRegisterType<bb::pim::CallType>("bb.cascades",1,3,"CallType");
	qmlRegisterType<bb::pim::SortOrder>("bb.cascades",1,3,"SortOrder");
	qmlRegisterType<bb::pim::Account>("bb.cascades",1,3,"Account");
	qmlRegisterType<bb::pim::AccountsChanged>("bb.cascades",1,3,"AccountsChanged");
	qmlRegisterType<bb::pim::AccountService>("bb.cascades",1,3,"AccountService");
	qmlRegisterType<bb::pim::Property>("bb.cascades",1,3,"Property");
	qmlRegisterType<bb::pim::Provider>("bb.cascades",1,3,"Provider");
	qmlRegisterType<bb::pim::Result>("bb.cascades",1,3,"Result");
	qmlRegisterType<bb::pim::Service>("bb.cascades",1,3,"Service");
	qmlRegisterType<bb::utility::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::utility::ImageConverter>("bb.cascades",1,3,"ImageConverter");
	qmlRegisterType<bb::utility::SpellChecker>("bb.cascades",1,3,"SpellChecker");
	qmlRegisterType<bb::utility::StringValidator>("bb.cascades",1,3,"StringValidator");
	qmlRegisterType<bb::utility::ClockFormat>("bb.cascades",1,3,"ClockFormat");
	qmlRegisterType<bb::utility::CustomDateFormatter>("bb.cascades",1,3,"CustomDateFormatter");
	qmlRegisterType<bb::utility::DateFormat>("bb.cascades",1,3,"DateFormat");
	qmlRegisterType<bb::utility::Formatting>("bb.cascades",1,3,"Formatting");
	qmlRegisterType<bb::utility::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::utility::RelativeDateFormat>("bb.cascades",1,3,"RelativeDateFormat");
	qmlRegisterType<bb::utility::RelativeDateFormatter>("bb.cascades",1,3,"RelativeDateFormatter");
	qmlRegisterType<bb::system::ApplicationStartupMode>("bb.cascades",1,3,"ApplicationStartupMode");
	qmlRegisterType<bb::system::CardDoneMessage>("bb.cascades",1,3,"CardDoneMessage");
	qmlRegisterType<bb::system::CardEdge>("bb.cascades",1,3,"CardEdge");
	qmlRegisterType<bb::system::CardOrientation>("bb.cascades",1,3,"CardOrientation");
	qmlRegisterType<bb::system::CardPeek>("bb.cascades",1,3,"CardPeek");
	qmlRegisterType<bb::system::CardResizeMessage>("bb.cascades",1,3,"CardResizeMessage");
	qmlRegisterType<bb::system::Clipboard>("bb.cascades",1,3,"Clipboard");
	qmlRegisterType<bb::system::ContextActivityState>("bb.cascades",1,3,"ContextActivityState");
	qmlRegisterType<bb::system::ContextClassification>("bb.cascades",1,3,"ContextClassification");
	qmlRegisterType<bb::system::ContextCollector>("bb.cascades",1,3,"ContextCollector");
	qmlRegisterType<bb::system::ContextEvent>("bb.cascades",1,3,"ContextEvent");
	qmlRegisterType<bb::system::FileTransferMode>("bb.cascades",1,3,"FileTransferMode");
	qmlRegisterType<bb::system::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::system::InvokeAction>("bb.cascades",1,3,"InvokeAction");
	qmlRegisterType<bb::system::InvokeDateTime>("bb.cascades",1,3,"InvokeDateTime");
	qmlRegisterType<bb::system::InvokeListCursorDirection>("bb.cascades",1,3,"InvokeListCursorDirection");
	qmlRegisterType<bb::system::InvokeManager>("bb.cascades",1,3,"InvokeManager");
	qmlRegisterType<bb::system::InvokeQueryTargetsReply>("bb.cascades",1,3,"InvokeQueryTargetsReply");
	qmlRegisterType<bb::system::InvokeQueryTargetsRequest>("bb.cascades",1,3,"InvokeQueryTargetsRequest");
	qmlRegisterType<bb::system::InvokeRecurrenceRule>("bb.cascades",1,3,"InvokeRecurrenceRule");
	qmlRegisterType<bb::system::InvokeRecurrenceRuleFrequency>("bb.cascades",1,3,"InvokeRecurrenceRuleFrequency");
	qmlRegisterType<bb::system::InvokeRecurrenceRuleLimit>("bb.cascades",1,3,"InvokeRecurrenceRuleLimit");
	qmlRegisterType<bb::system::InvokeReply>("bb.cascades",1,3,"InvokeReply");
	qmlRegisterType<bb::system::InvokeReplyError>("bb.cascades",1,3,"InvokeReplyError");
	qmlRegisterType<bb::system::InvokeRequest>("bb.cascades",1,3,"InvokeRequest");
	qmlRegisterType<bb::system::InvokeRequestTargetFiltersReply>("bb.cascades",1,3,"InvokeRequestTargetFiltersReply");
	qmlRegisterType<bb::system::InvokeSource>("bb.cascades",1,3,"InvokeSource");
	qmlRegisterType<bb::system::InvokeTarget>("bb.cascades",1,3,"InvokeTarget");
	qmlRegisterType<bb::system::InvokeTargetFilter>("bb.cascades",1,3,"InvokeTargetFilter");
	qmlRegisterType<bb::system::InvokeTargetReply>("bb.cascades",1,3,"InvokeTargetReply");
	qmlRegisterType<bb::system::InvokeTimerRequest>("bb.cascades",1,3,"InvokeTimerRequest");
	qmlRegisterType<bb::system::InvokeUpdateTargetFiltersRequest>("bb.cascades",1,3,"InvokeUpdateTargetFiltersRequest");
	qmlRegisterType<bb::system::ListSelectionIndicator>("bb.cascades",1,3,"ListSelectionIndicator");
	qmlRegisterType<bb::system::ListSelectionMode>("bb.cascades",1,3,"ListSelectionMode");
	qmlRegisterType<bb::system::LocaleHandler>("bb.cascades",1,3,"LocaleHandler");
	qmlRegisterType<bb::system::LocaleType>("bb.cascades",1,3,"LocaleType");
	qmlRegisterType<bb::system::MeasurementSystem>("bb.cascades",1,3,"MeasurementSystem");
	qmlRegisterType<bb::system::Menu>("bb.cascades",1,3,"Menu");
	qmlRegisterType<bb::system::MenuItem>("bb.cascades",1,3,"MenuItem");
	qmlRegisterType<bb::system::MenuItemInvokeParams>("bb.cascades",1,3,"MenuItemInvokeParams");
	qmlRegisterType<bb::system::MenuItemType>("bb.cascades",1,3,"MenuItemType");
	qmlRegisterType<bb::system::MenuManager>("bb.cascades",1,3,"MenuManager");
	qmlRegisterType<bb::system::MenuManagerError>("bb.cascades",1,3,"MenuManagerError");
	qmlRegisterType<bb::system::NfcShareDataContent>("bb.cascades",1,3,"NfcShareDataContent");
	qmlRegisterType<bb::system::NfcShareError>("bb.cascades",1,3,"NfcShareError");
	qmlRegisterType<bb::system::NfcShareFilesContent>("bb.cascades",1,3,"NfcShareFilesContent");
	qmlRegisterType<bb::system::NfcShareHandoverTransport>("bb.cascades",1,3,"NfcShareHandoverTransport");
	qmlRegisterType<bb::system::NfcShareManager>("bb.cascades",1,3,"NfcShareManager");
	qmlRegisterType<bb::system::NfcShareMode>("bb.cascades",1,3,"NfcShareMode");
	qmlRegisterType<bb::system::NfcShareSetContentError>("bb.cascades",1,3,"NfcShareSetContentError");
	qmlRegisterType<bb::system::NfcShareStartTransferMode>("bb.cascades",1,3,"NfcShareStartTransferMode");
	qmlRegisterType<bb::system::NfcShareSuccess>("bb.cascades",1,3,"NfcShareSuccess");
	qmlRegisterType<bb::system::Screenshot>("bb.cascades",1,3,"Screenshot");
	qmlRegisterType<bb::system::ScreenshotError>("bb.cascades",1,3,"ScreenshotError");
	qmlRegisterType<bb::system::ScreenshotFormat>("bb.cascades",1,3,"ScreenshotFormat");
	qmlRegisterType<bb::system::SecurityPerimeter>("bb.cascades",1,3,"SecurityPerimeter");
	qmlRegisterType<bb::system::SystemCredentialsPrompt>("bb.cascades",1,3,"SystemCredentialsPrompt");
	qmlRegisterType<bb::system::SystemDialog>("bb.cascades",1,3,"SystemDialog");
	qmlRegisterType<bb::system::SystemListDialog>("bb.cascades",1,3,"SystemListDialog");
	qmlRegisterType<bb::system::SystemProgressDialog>("bb.cascades",1,3,"SystemProgressDialog");
	qmlRegisterType<bb::system::SystemProgressToast>("bb.cascades",1,3,"SystemProgressToast");
	qmlRegisterType<bb::system::SystemPrompt>("bb.cascades",1,3,"SystemPrompt");
	qmlRegisterType<bb::system::SystemToast>("bb.cascades",1,3,"SystemToast");
	qmlRegisterType<bb::system::SystemUiButton>("bb.cascades",1,3,"SystemUiButton");
	qmlRegisterType<bb::system::SystemUiError>("bb.cascades",1,3,"SystemUiError");
	qmlRegisterType<bb::system::SystemUiInputField>("bb.cascades",1,3,"SystemUiInputField");
	qmlRegisterType<bb::system::SystemUiInputMode>("bb.cascades",1,3,"SystemUiInputMode");
	qmlRegisterType<bb::system::SystemUiInputOption>("bb.cascades",1,3,"SystemUiInputOption");
	qmlRegisterType<bb::system::SystemUiModality>("bb.cascades",1,3,"SystemUiModality");
	qmlRegisterType<bb::system::SystemUiPosition>("bb.cascades",1,3,"SystemUiPosition");
	qmlRegisterType<bb::system::SystemUiProgressState>("bb.cascades",1,3,"SystemUiProgressState");
	qmlRegisterType<bb::system::SystemUiResult>("bb.cascades",1,3,"SystemUiResult");
	qmlRegisterType<bb::system::SystemUiReturnKeyAction>("bb.cascades",1,3,"SystemUiReturnKeyAction");
	qmlRegisterType<bb::system::Call>("bb.cascades",1,3,"Call");
	qmlRegisterType<bb::system::CallAction>("bb.cascades",1,3,"CallAction");
	qmlRegisterType<bb::system::CallCommandResponse>("bb.cascades",1,3,"CallCommandResponse");
	qmlRegisterType<bb::system::CallState>("bb.cascades",1,3,"CallState");
	qmlRegisterType<bb::system::CallType>("bb.cascades",1,3,"CallType");
	qmlRegisterType<bb::system::Line>("bb.cascades",1,3,"Line");
	qmlRegisterType<bb::system::LineType>("bb.cascades",1,3,"LineType");
	qmlRegisterType<bb::system::Phone>("bb.cascades",1,3,"Phone");
	qmlRegisterType<bb::device::BatteryChargingState>("bb.cascades",1,3,"BatteryChargingState");
	qmlRegisterType<bb::device::BatteryCondition>("bb.cascades",1,3,"BatteryCondition");
	qmlRegisterType<bb::device::BatteryInfo>("bb.cascades",1,3,"BatteryInfo");
	qmlRegisterType<bb::device::CellularBand>("bb.cascades",1,3,"CellularBand");
	qmlRegisterType<bb::device::CellularConnectionRequestResult>("bb.cascades",1,3,"CellularConnectionRequestResult");
	qmlRegisterType<bb::device::CellularConnectionState>("bb.cascades",1,3,"CellularConnectionState");
	qmlRegisterType<bb::device::CellularDataInterface>("bb.cascades",1,3,"CellularDataInterface");
	qmlRegisterType<bb::device::CellularNetworkInfo>("bb.cascades",1,3,"CellularNetworkInfo");
	qmlRegisterType<bb::device::CellularRadioInfo>("bb.cascades",1,3,"CellularRadioInfo");
	qmlRegisterType<bb::device::CellularService>("bb.cascades",1,3,"CellularService");
	qmlRegisterType<bb::device::CellularTechnology>("bb.cascades",1,3,"CellularTechnology");
	qmlRegisterType<bb::device::DeviceInfo>("bb.cascades",1,3,"DeviceInfo");
	qmlRegisterType<bb::device::DeviceOrientation>("bb.cascades",1,3,"DeviceOrientation");
	qmlRegisterType<bb::device::DisplayAspectType>("bb.cascades",1,3,"DisplayAspectType");
	qmlRegisterType<bb::device::DisplayInfo>("bb.cascades",1,3,"DisplayInfo");
	qmlRegisterType<bb::device::DisplayTechnology>("bb.cascades",1,3,"DisplayTechnology");
	qmlRegisterType<bb::device::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::device::HardwareInfo>("bb.cascades",1,3,"HardwareInfo");
	qmlRegisterType<bb::device::HdmiConnector>("bb.cascades",1,3,"HdmiConnector");
	qmlRegisterType<bb::device::Led>("bb.cascades",1,3,"Led");
	qmlRegisterType<bb::device::LedColor>("bb.cascades",1,3,"LedColor");
	qmlRegisterType<bb::device::NetworkDataUsage>("bb.cascades",1,3,"NetworkDataUsage");
	qmlRegisterType<bb::device::SdCardActivity>("bb.cascades",1,3,"SdCardActivity");
	qmlRegisterType<bb::device::SdCardInfo>("bb.cascades",1,3,"SdCardInfo");
	qmlRegisterType<bb::device::SdCardState>("bb.cascades",1,3,"SdCardState");
	qmlRegisterType<bb::device::SimCardInfo>("bb.cascades",1,3,"SimCardInfo");
	qmlRegisterType<bb::device::SimCardState>("bb.cascades",1,3,"SimCardState");
	qmlRegisterType<bb::device::UserActivityState>("bb.cascades",1,3,"UserActivityState");
	qmlRegisterType<bb::device::VibrationController>("bb.cascades",1,3,"VibrationController");
	qmlRegisterType<bb::device::WiFiDirect>("bb.cascades",1,3,"WiFiDirect");
	qmlRegisterType<bb::device::WiFiDirectConnectionState>("bb.cascades",1,3,"WiFiDirectConnectionState");
	qmlRegisterType<bb::device::WiFiDirectDeviceType>("bb.cascades",1,3,"WiFiDirectDeviceType");
	qmlRegisterType<bb::device::WiFiDirectIntent>("bb.cascades",1,3,"WiFiDirectIntent");
	qmlRegisterType<bb::device::WiFiDirectResult>("bb.cascades",1,3,"WiFiDirectResult");
	qmlRegisterType<bb::device::WiFiDirectRole>("bb.cascades",1,3,"WiFiDirectRole");
	qmlRegisterType<bb::device::WiFiDirectSecondaryDeviceType>("bb.cascades",1,3,"WiFiDirectSecondaryDeviceType");
	qmlRegisterType<bb::device::WiFiDirectWpsMethod>("bb.cascades",1,3,"WiFiDirectWpsMethod");
	qmlRegisterType<bb::data::AsyncDataAccess>("bb.cascades",1,3,"AsyncDataAccess");
	qmlRegisterType<bb::data::AsyncWorker>("bb.cascades",1,3,"AsyncWorker");
	qmlRegisterType<bb::data::DataAccessError>("bb.cascades",1,3,"DataAccessError");
	qmlRegisterType<bb::data::DataAccessErrorType>("bb.cascades",1,3,"DataAccessErrorType");
	qmlRegisterType<bb::data::DataAccessReply>("bb.cascades",1,3,"DataAccessReply");
	qmlRegisterType<bb::data::DataSource>("bb.cascades",1,3,"DataSource");
	qmlRegisterType<bb::data::DataSourceType>("bb.cascades",1,3,"DataSourceType");
	qmlRegisterType<bb::data::JsonDataAccess>("bb.cascades",1,3,"JsonDataAccess");
	qmlRegisterType<bb::data::SqlConnection>("bb.cascades",1,3,"SqlConnection");
	qmlRegisterType<bb::data::SqlDataAccess>("bb.cascades",1,3,"SqlDataAccess");
	qmlRegisterType<bb::data::SqlWorker>("bb.cascades",1,3,"SqlWorker");
	qmlRegisterType<bb::data::XmlDataAccess>("bb.cascades",1,3,"XmlDataAccess");
	qmlRegisterType<bb::platform::CancelSubscriptionReply>("bb.cascades",1,3,"CancelSubscriptionReply");
	qmlRegisterType<bb::platform::DeviceLockState>("bb.cascades",1,3,"DeviceLockState");
	qmlRegisterType<bb::platform::DigitalGoodReply>("bb.cascades",1,3,"DigitalGoodReply");
	qmlRegisterType<bb::platform::DigitalGoodState>("bb.cascades",1,3,"DigitalGoodState");
	qmlRegisterType<bb::platform::ExistingPurchasesReply>("bb.cascades",1,3,"ExistingPurchasesReply");
	qmlRegisterType<bb::platform::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::platform::HandsFreeNavigation>("bb.cascades",1,3,"HandsFreeNavigation");
	qmlRegisterType<bb::platform::HomeScreen>("bb.cascades",1,3,"HomeScreen");
	qmlRegisterType<bb::platform::LocationMapInvoker>("bb.cascades",1,3,"LocationMapInvoker");
	qmlRegisterType<bb::platform::MapInvoker>("bb.cascades",1,3,"MapInvoker");
	qmlRegisterType<bb::platform::MapNavigationMode>("bb.cascades",1,3,"MapNavigationMode");
	qmlRegisterType<bb::platform::MapSearchMode>("bb.cascades",1,3,"MapSearchMode");
	qmlRegisterType<bb::platform::MapTransportationMode>("bb.cascades",1,3,"MapTransportationMode");
	qmlRegisterType<bb::platform::Notification>("bb.cascades",1,3,"Notification");
	qmlRegisterType<bb::platform::NotificationApplicationSettings>("bb.cascades",1,3,"NotificationApplicationSettings");
	qmlRegisterType<bb::platform::NotificationDefaultApplicationSettings>("bb.cascades",1,3,"NotificationDefaultApplicationSettings");
	qmlRegisterType<bb::platform::NotificationDialog>("bb.cascades",1,3,"NotificationDialog");
	qmlRegisterType<bb::platform::NotificationError>("bb.cascades",1,3,"NotificationError");
	qmlRegisterType<bb::platform::NotificationGlobalSettings>("bb.cascades",1,3,"NotificationGlobalSettings");
	qmlRegisterType<bb::platform::NotificationMode>("bb.cascades",1,3,"NotificationMode");
	qmlRegisterType<bb::platform::NotificationPolicy>("bb.cascades",1,3,"NotificationPolicy");
	qmlRegisterType<bb::platform::NotificationPriorityPolicy>("bb.cascades",1,3,"NotificationPriorityPolicy");
	qmlRegisterType<bb::platform::NotificationResult>("bb.cascades",1,3,"NotificationResult");
	qmlRegisterType<bb::platform::NotificationSettingsError>("bb.cascades",1,3,"NotificationSettingsError");
	qmlRegisterType<bb::platform::NotificationType>("bb.cascades",1,3,"NotificationType");
	qmlRegisterType<bb::platform::PaymentConnectionMode>("bb.cascades",1,3,"PaymentConnectionMode");
	qmlRegisterType<bb::platform::PaymentErrorCode>("bb.cascades",1,3,"PaymentErrorCode");
	qmlRegisterType<bb::platform::PaymentManager>("bb.cascades",1,3,"PaymentManager");
	qmlRegisterType<bb::platform::PaymentReply>("bb.cascades",1,3,"PaymentReply");
	qmlRegisterType<bb::platform::PlatformInfo>("bb.cascades",1,3,"PlatformInfo");
	qmlRegisterType<bb::platform::PriceReply>("bb.cascades",1,3,"PriceReply");
	qmlRegisterType<bb::platform::PurchaseReceipt>("bb.cascades",1,3,"PurchaseReceipt");
	qmlRegisterType<bb::platform::PurchaseReply>("bb.cascades",1,3,"PurchaseReply");
	qmlRegisterType<bb::platform::RouteMapInvoker>("bb.cascades",1,3,"RouteMapInvoker");
	qmlRegisterType<bb::platform::SubscriptionStatusReply>("bb.cascades",1,3,"SubscriptionStatusReply");
	qmlRegisterType<bb::platform::SubscriptionTermsReply>("bb.cascades",1,3,"SubscriptionTermsReply");
	qmlRegisterType<bb::platform::TypicalMapAltitude>("bb.cascades",1,3,"TypicalMapAltitude");
	qmlRegisterType<bb::platform::WallpaperLockState>("bb.cascades",1,3,"WallpaperLockState");
	qmlRegisterType<bb::platform::WallpaperResult>("bb.cascades",1,3,"WallpaperResult");
	qmlRegisterType<bb::platform::BlackBerryId>("bb.cascades",1,3,"BlackBerryId");
	qmlRegisterType<bb::platform::BlackBerryIdAssuranceLevel>("bb.cascades",1,3,"BlackBerryIdAssuranceLevel");
	qmlRegisterType<bb::platform::BlackBerryIdChallengeFlag>("bb.cascades",1,3,"BlackBerryIdChallengeFlag");
	qmlRegisterType<bb::platform::BlackBerryIdChallengeType>("bb.cascades",1,3,"BlackBerryIdChallengeType");
	qmlRegisterType<bb::platform::BlackBerryIdPropertyType>("bb.cascades",1,3,"BlackBerryIdPropertyType");
	qmlRegisterType<bb::platform::BlackBerryProfile>("bb.cascades",1,3,"BlackBerryProfile");
	qmlRegisterType<bb::platform::BlackBerryProfileCreateDataFlag>("bb.cascades",1,3,"BlackBerryProfileCreateDataFlag");
	qmlRegisterType<bb::platform::BlackBerryProfileDeleteDataFlag>("bb.cascades",1,3,"BlackBerryProfileDeleteDataFlag");
	qmlRegisterType<bb::platform::BlackBerryProfileGetDataFlag>("bb.cascades",1,3,"BlackBerryProfileGetDataFlag");
	qmlRegisterType<bb::platform::BlackBerryProfileListDataFlag>("bb.cascades",1,3,"BlackBerryProfileListDataFlag");
	qmlRegisterType<bb::platform::BlackBerryProfileNotificationFlag>("bb.cascades",1,3,"BlackBerryProfileNotificationFlag");
	qmlRegisterType<bb::platform::BlackBerryProfilePropertyType>("bb.cascades",1,3,"BlackBerryProfilePropertyType");
	qmlRegisterType<bb::platform::BlackBerryProfileRegisterNotifierFlag>("bb.cascades",1,3,"BlackBerryProfileRegisterNotifierFlag");
	qmlRegisterType<bb::platform::BlackBerryProfileSetDataFlag>("bb.cascades",1,3,"BlackBerryProfileSetDataFlag");
	qmlRegisterType<bb::platform::IdentityService>("bb.cascades",1,3,"IdentityService");
	qmlRegisterType<bb::platform::IdentityServiceLogVerbosity>("bb.cascades",1,3,"IdentityServiceLogVerbosity");
	qmlRegisterType<bb::platform::IdentityServicePropertyResult>("bb.cascades",1,3,"IdentityServicePropertyResult");
	qmlRegisterType<bb::platform::IdentityServiceProvider>("bb.cascades",1,3,"IdentityServiceProvider");
	qmlRegisterType<bb::platform::IdentityServiceResult>("bb.cascades",1,3,"IdentityServiceResult");
	qmlRegisterType<bb::platform::IdentityServiceTokenResult>("bb.cascades",1,3,"IdentityServiceTokenResult");
	qmlRegisterType<bb::platform::ApplicationPermissions>("bb.cascades",1,3,"ApplicationPermissions");
	qmlRegisterType<bb::platform::Contact>("bb.cascades",1,3,"Contact");
	qmlRegisterType<bb::platform::ContactService>("bb.cascades",1,3,"ContactService");
	qmlRegisterType<bb::platform::Context>("bb.cascades",1,3,"Context");
	qmlRegisterType<bb::platform::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::platform::ImageType>("bb.cascades",1,3,"ImageType");
	qmlRegisterType<bb::platform::MessageService>("bb.cascades",1,3,"MessageService");
	qmlRegisterType<bb::platform::Permission>("bb.cascades",1,3,"Permission");
	qmlRegisterType<bb::platform::PermissionValue>("bb.cascades",1,3,"PermissionValue");
	qmlRegisterType<bb::platform::ProfileBox>("bb.cascades",1,3,"ProfileBox");
	qmlRegisterType<bb::platform::ProfileBoxItem>("bb.cascades",1,3,"ProfileBoxItem");
	qmlRegisterType<bb::platform::RegistrationState>("bb.cascades",1,3,"RegistrationState");
	qmlRegisterType<bb::platform::UserProfile>("bb.cascades",1,3,"UserProfile");
	qmlRegisterType<bb::platform::UserStatus>("bb.cascades",1,3,"UserStatus");
	qmlRegisterType<bb::platform::AltitudeMode>("bb.cascades",1,3,"AltitudeMode");
	qmlRegisterType<bb::platform::BoundingBox>("bb.cascades",1,3,"BoundingBox");
	qmlRegisterType<bb::platform::Coordinate>("bb.cascades",1,3,"Coordinate");
	qmlRegisterType<bb::platform::EdgeSize>("bb.cascades",1,3,"EdgeSize");
	qmlRegisterType<bb::platform::EdgeStyle>("bb.cascades",1,3,"EdgeStyle");
	qmlRegisterType<bb::platform::GeoDeviceLocation>("bb.cascades",1,3,"GeoDeviceLocation");
	qmlRegisterType<bb::platform::Geographic>("bb.cascades",1,3,"Geographic");
	qmlRegisterType<bb::platform::GeoList>("bb.cascades",1,3,"GeoList");
	qmlRegisterType<bb::platform::GeoLocation>("bb.cascades",1,3,"GeoLocation");
	qmlRegisterType<bb::platform::GeoPolygon>("bb.cascades",1,3,"GeoPolygon");
	qmlRegisterType<bb::platform::GeoPolyline>("bb.cascades",1,3,"GeoPolyline");
	qmlRegisterType<bb::platform::GeoShape>("bb.cascades",1,3,"GeoShape");
	qmlRegisterType<bb::platform::Global>("bb.cascades",1,3,"Global");
	qmlRegisterType<bb::platform::Marker>("bb.cascades",1,3,"Marker");
	qmlRegisterType<bb::platform::Point>("bb.cascades",1,3,"Point");
	qmlRegisterType<bb::platform::Polyline>("bb.cascades",1,3,"Polyline");
	qmlRegisterType<bb::platform::Style>("bb.cascades",1,3,"Style");
	qmlRegisterType<bb::platform::StyleSheet>("bb.cascades",1,3,"StyleSheet");
}

#endif //QTCREATOR_CASCADES_QML_COMPLETION
