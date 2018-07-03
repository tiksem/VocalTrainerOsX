//
// Created by Semyon Tikhonenko on 7/2/18.
// Copyright (c) 2018 Mac. All rights reserved.
//

#import "WorkspaceView.h"
#include "WorkspaceDrawer.h"
#include "QuartzDrawer.h"
#import "UiUtils.h"


@implementation WorkspaceView {
    std::unique_ptr<WorkspaceDrawer> _workspaceDrawer;
}

- (instancetype)initWithCoder:(nonnull NSCoder *)coder {
    self = [super initWithCoder:coder];
    if (self) {
        [UiUtils setupFrameEventOfView:self];
    }

    return self;
}

- (void)frameChanged:(id)boundsChanged {
    [self resizeDrawer];
    [self setNeedsDisplay:YES];
}

- (void)drawRect:(NSRect)dirtyRect {
    [super drawRect:dirtyRect];
    if (!_workspaceDrawer) {
        Drawer* drawer = new QuartzDrawer();
        _workspaceDrawer = std::make_unique<WorkspaceDrawer>(drawer);
        _workspaceDrawer->setIntervalWidth(30);
        _workspaceDrawer->setIntervalHeight(15);
        [self resizeDrawer];
    }

    _workspaceDrawer->draw();
}

- (void)resizeDrawer {
    if (!_workspaceDrawer) {
        return;
    }

    CGFloat width = self.frame.size.width;
    CGFloat height = self.frame.size.height;
    _workspaceDrawer->resize(width, height, 2);
}

@end
