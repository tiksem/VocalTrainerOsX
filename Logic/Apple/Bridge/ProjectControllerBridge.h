//
// Created by Semyon Tykhonenko on 2019-07-27.
// Copyright (c) 2019 Semyon Tikhonenko. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <CoreGraphics/CoreGraphics.h>
#import <Logic/Logic-Swift.h>

@protocol ProjectControllerBridgeDelegate;


@interface ProjectControllerBridge : NSObject
- (instancetype)init;
- (void)addDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(add(delegate:));
- (void)removeDelegate:(id<ProjectControllerBridgeDelegate>)delegate NS_SWIFT_NAME(remove(delegate:));

- (void)setWorkspaceController:(void*)workspaceController;

@property (readonly) NSString* artistName;
@property (readonly) NSString* songTitle;

@property (readonly) float maxZoom;
@property (readonly) float minZoom;
@property (nonatomic) float zoom;
- (void)setZoom:(float)zoom intoPoint:(CGPoint)point;
@property (nonatomic) float verticalScrollPosition;

@property (readonly) bool lyricsVisible;
@property (readonly) bool tracksVisible;
@property (readonly) bool metronomeEnabled;
@property (readonly) bool boundsSelectionEnabled;
@property (readonly) bool isPlaying;
@property (readonly) NSArray<LyricsSection*>* lyricsSections;

- (void)goToBeginning;
- (void)toggleTracksVisibility;
- (void)toggleLyricsVisibility;
- (void)toggleMetronomeEnabled;
- (void)toggleBoundsSelectionEnabled;

- (void)togglePlay;
- (void)stop;
- (void)toggleRewindWithBackward:(BOOL)backward;

- (void)setPlaybackProgress:(double)value;
- (double)convertSeekToPlaybackProgress:(double)seek;

- (void)setVocalVolume:(float)value;
- (void)setInputSensitivity:(float)value;
- (void)setVocalPianoVolume:(float)value;
- (void)setInstrumentalVolume:(float)value;
@end