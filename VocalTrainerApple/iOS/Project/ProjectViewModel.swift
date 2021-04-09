//
// Created by Semyon Tikhonenko on 12/18/20.
// Copyright (c) 2020 Semyon Tikhonenko. All rights reserved.
//

import Combine
import AVFoundation

class ProjectViewModel : ObservableObject {
    private let audioEngine = AVAudioEngine()

    @Published var isMetronomeEnabled = false {
        didSet {
            projectController.metronomeEnabled = isMetronomeEnabled
        }
    }
    @Published var isLyricsVisible = false {
        didSet {
            projectController.lyricsVisible = isLyricsVisible
        }
    }
    @Published var lyricsLines: [String] = []
    @Published var lyricsSelection = LyricsSelection(characterIndex: 0, position: 0, lineIndex: 0)
    @Published var playbackSections: [PlaybackSection] = []
    @Published var title: String = ""
    @Published var playbackCurrentTime: String = ""
    @Published var playbackEndTime: String = ""
    @Published private(set) var retrySeconds = 5
    @Published private(set) var isPlaying: Bool = false

    private let timeFormatter = DateFormatter()

    private var disableProgressUpdate = false
    @Published var progress: CGFloat = 0 {
        didSet {
            if oldValue != progress && !disableProgressUpdate {
                projectController.setPlaybackProgress(Double(progress))
            }
        }
    }

    var originalTonality : String {
        get {
            ProjectController.shared.originalTonality.toString()
        }
    }

    private lazy var projectController = ProjectController.shared

    private func updatePlaybackEndTime() {
        playbackEndTime = timeFormatter.string(from: Date(timeIntervalSince1970: projectController.duration))
    }

    init() {
        isMetronomeEnabled = projectController.metronomeEnabled
        projectController.add(delegate: self)
        let lyricsSections: [LyricsSection] = projectController.lyricsSections
        playbackSections = lyricsSections.map {
            let type = $0.type
            let name = Strings.from(sectionType: type).localized
            let position = CGFloat(projectController.convertSeek(toPlaybackProgress: $0.seek))
            return PlaybackSection(name: name, position: position)
        }
        title = projectController.artistName + " - " + projectController.songTitle
        timeFormatter.dateFormat = "m:ss"
        updatePlaybackEndTime()

        audioEngine.connect(
                audioEngine.inputNode,
                to: audioEngine.outputNode,
                format: audioEngine.inputNode.inputFormat(forBus: 0))
    }

    deinit {
        projectController.remove(delegate: self)
    }

    func willBecomeInactive() {
        ProjectController.shared.stop()
    }
    
    func didBecomeActive() {

    }
    
    func didTapPlayButton() {
        ProjectController.shared.togglePlay()
    }

    func didTapRetry() {
        projectController.rewindBack(bySeconds: Double(retrySeconds))
    }
}

extension ProjectViewModel : ProjectControllerBridgeDelegate {
    public func projectController(didChangeMetronomeEnabled enabled: Bool) {
        self.isMetronomeEnabled = enabled
    }

    public func projectControllerUpdate(currentLyricsLines: [String]) {
        self.lyricsLines = currentLyricsLines;
    }

    func projectControllerUpdateLyricsSelection(
            selectedCharactersCount: Int,
            lastCharacterSelectionPosition: CGFloat,
            lineIndex: Int
    ) {
        lyricsSelection = LyricsSelection(
                characterIndex: selectedCharactersCount == 0 ? 0 : selectedCharactersCount - 1,
                position: lastCharacterSelectionPosition,
                lineIndex: lineIndex)
    }

    func projectControllerUpdate(seek: Double) {
        disableProgressUpdate = true
        progress = CGFloat(projectController.convertSeek(toPlaybackProgress: seek))
        disableProgressUpdate = false
        playbackCurrentTime = timeFormatter.string(from: Date(timeIntervalSince1970: seek))
    }

    func projectControllerUpdate(lyricsVisibility: Bool) {
        isLyricsVisible = lyricsVisibility;
    }

    func projectControllerUpdateTempo(factor: Double) {
        updatePlaybackEndTime()
    }

    func projectControllerPlaybackDidStart() {
        try! audioEngine.start()
        isPlaying = true
    }

    func projectControllerPlaybackDidStop() {
        audioEngine.stop()
        isPlaying = false
    }
}
