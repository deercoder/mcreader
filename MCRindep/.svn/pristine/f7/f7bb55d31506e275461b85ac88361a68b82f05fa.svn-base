package mcr.indep;

import java.util.List;

import android.app.ProgressDialog;
import android.content.Context;
import android.hardware.Camera;
import android.hardware.Camera.Size;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

//Implements the preview of the camera before you take the picture
class Preview extends SurfaceView implements SurfaceHolder.Callback {
	private static final String TAG = "MCR_PREVIEW_DEBUG";
	public ProgressDialog dialog;
	
    public SurfaceHolder mHolder;
    public Camera mCamera;
    
    /*Size mPreviewSize;
    List<Size> mSupportedPreviewSizes;*/

    
    Preview(Context context) {
        super(context);        
        // Install a SurfaceHolder.Callback so we get notified when the
        // underlying surface is created and destroyed.
        Log.d(TAG, "Preview created");
        mHolder = getHolder();
        mHolder.addCallback(this);
        mHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    }
    
    // Shows the dialog while waiting for image processing
    public void progressDialog () {
    	Log.d(TAG, "Starting the \"processing\" dialog.");
    	mCamera.stopPreview();
    	dialog = ProgressDialog.show(this.getContext(), "Processing", "Please wait for image to process...", true);
    }
    
    /*public void setCamera(Camera cam) {
    	mCamera = cam;
    	
    	//Use preset values for width and height
		Camera.Parameters parameters = mCamera.getParameters();
        parameters.setPreviewSize(Constants.PREVIEW_WIDTH, Constants.PREVIEW_HEIGHT);
        mCamera.setParameters(parameters);
    }*/
    
    public void surfaceCreated(SurfaceHolder holder) {
        // The Surface has been created, acquire the camera and tell it where
        // to draw.
    	Log.d(TAG, "surface created");

        mCamera = Camera.open();
        
        try {
        	mCamera.setPreviewDisplay(holder);
		} catch (Exception e) {
			e.printStackTrace();
		}
    }
    
    public void refresh() {
    	if (mCamera != null) {
    		Camera.Parameters parameters = mCamera.getParameters();
    		if (parameters != null && parameters.getSupportedPreviewSizes() != null) {
	            for (Size s : parameters.getSupportedPreviewSizes()) {
	            	Log.d(TAG, s.width + "x" + s.height);
	            }
	            
	            //Hide the dialog since we will be starting camera preview
	            if( dialog != null ) {
		            dialog.hide();
		            dialog = null;
	            }
	            
	            //Already chosen preset values for width and height
	            parameters.setPreviewSize(Constants.PREVIEW_WIDTH, Constants.PREVIEW_HEIGHT);
	            mCamera.setParameters(parameters);
	            
	            //Start the preview
	            mCamera.startPreview();
    		}
    	}
    }

    public void surfaceChanged(SurfaceHolder holder, int format, int w, int h) {
        // Set up the camera parameters and begin the preview.
    	Log.d(TAG, "Surface changed");
    	Camera.Parameters parameters = mCamera.getParameters();
        
    	//getSupportedPreviewSizes and choose the one the fits our requirements
    	/*int width = 0;
    	int height = 0;
    	for (Size s : parameters.getSupportedPreviewSizes()) {
			Log.d(TAG, "available size: " + String.valueOf(s.height) + ","+ String.valueOf(s.width));
			width = s.width;
			height = s.height;
			if ( width > Constants.PICTURE_WIDTH_THRESHOLD || height > Constants.PICTURE_HEIGHT_THRESHOLD) {
				break;
			}
		}*/
    	
        //Already chosen preset values for width and height
        parameters.setPreviewSize(Constants.PREVIEW_WIDTH, Constants.PREVIEW_HEIGHT);
        mCamera.setParameters(parameters);
        
        //Start the preview
        mCamera.startPreview();
    }

    public void surfaceDestroyed(SurfaceHolder holder) {
        // Surface will be destroyed when we return, so stop the preview.
        // Because the CameraDevice object is not a shared resource, it's very
        // important to release it when the activity is paused.
    	Log.d(TAG, "Preview surface destroyed, releasing camera.");
    	
		if( dialog != null )
			dialog.dismiss();
    	
		if( mCamera != null ) {
	        mCamera.stopPreview();
	        mCamera.release();
	        mCamera = null;
		}
    }

}