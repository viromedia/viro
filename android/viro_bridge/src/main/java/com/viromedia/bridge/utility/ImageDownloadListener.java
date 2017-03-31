/*
 * Copyright © 2016 Viro Media. All rights reserved.
 */

package com.viromedia.bridge.utility;


import android.graphics.Bitmap;

public interface ImageDownloadListener {
    /**
     * Whether or not the download listener is still valid
     */
    public boolean isValid();
    public void completed(Bitmap result);
    public void failed(String error);
}
