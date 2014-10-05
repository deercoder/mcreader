/*
 * MCRActivity.java
 * Mobile Currency Reader
 * SS12 February 2010
 * 
 * Based on example camera code at http://marakana.com/forums/android/android_examples/39.html
 * 
 */

package mcr.indep;

import java.io.BufferedReader;
import java.io.DataInputStream;
import java.io.DataOutputStream;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.SocketException;
import java.net.SocketTimeoutException;
import java.net.URL;
import java.net.URLConnection;
import java.util.ArrayList;
import java.util.List;

import android.app.Activity;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.hardware.Camera.Size;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.Window;
import android.view.WindowManager;
import android.widget.FrameLayout;

public class MCRindepActivity extends Activity {
	private static final String TAG = "MCR_ACTIVITY_DEBUG";
	
	public Preview preview; // previews video display and also contains a camera object
	private MediaPlayer myMediaPlayer;

	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		
		//Hide the title window, must be done before setting the layout
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN);		//hides status bar
		
		setContentView(R.layout.main);
		
		// this R.id stuff is in the main.xml file
		// create the surface that the camera preview is on
		if (preview == null) {
			//Initialize the c++ code
			//initializeCurrencyReader();
			
			preview = new Preview(this);
			((FrameLayout) findViewById(R.id.preview)).addView(preview);
			
			// Start running image processing code
			Log.d(TAG, "Initializing JNI Code.");
			initializeCurrencyReader();			//Should execute earlier in its own thread
			
			preview.setOnClickListener( new OnClickListener() {
				// takes picture when the screen is touched/clicked
				public void onClick(View v) {
					// set photo resolution as preparation for image processing
					Camera.Parameters parameters = preview.mCamera.getParameters();
					int width = 0;
					int height = 0;
					//should get 1024 by 768
					if (parameters != null) {
						if (parameters.getSupportedPictureSizes() != null) {
							for (Size s : parameters.getSupportedPictureSizes()) {
								Log.d(TAG, "available size: " + String.valueOf(s.height) +","+ String.valueOf(s.width));
								width = s.width;
								height = s.height;
								if ( width > Constants.PICTURE_WIDTH_THRESHOLD || height > Constants.PICTURE_HEIGHT_THRESHOLD) {
									break;
								}
							}
							parameters.setPictureSize(width, height);
							preview.mCamera.setParameters(parameters);
						}
					}
					
					preview.mCamera.takePicture(shutterCallback, null, jpegCallback);
					//preview.mCamera.takePicture(null, rawCallback, jpegCallback);
				}
			});
		}
	}
	
	final Handler handler = new Handler() {
		public void handleMessage(Message msg) {
            int running = msg.getData().getInt("running");
            if (running == 0) {		//The image processing thread is no longer running			
            	preview.refresh();		//Refresh the preview the so user can take another pic
            }
        }

	};
	
	@Override
	public void onPause() {
		super.onPause();
		Log.d(TAG, "Going onPause!!!!!!!!!!!");
		if( preview.dialog != null )
			preview.dialog.dismiss();
		
		// Stop and release the camera
		if( preview.mCamera != null ) {
			preview.mCamera.stopPreview();
			preview.mCamera.release();
			preview.mCamera = null;
		}
		
	}
	
	@Override
	public void onResume() {
		super.onResume();
		Log.d(TAG, "Going onResume!!!!!!!!!!!");
		
		// Give the camera object back to the Preview class
		if( preview.mCamera != null && preview.mHolder!= null) {
	        preview.mCamera = Camera.open();
	        try {
	        	preview.mCamera.setPreviewDisplay(preview.mHolder);
			} catch (Exception e) {
				e.printStackTrace();
			}
		}
	}
	
	ShutterCallback shutterCallback = new ShutterCallback() {
		public void onShutter() {
			/* make shutter sound to signal that picture was taken */
			//playAudio(Constants.SHUTTER_SOUND);
			//playAudio(Constants.ONE_DOLLAR_SOUND);
			preview.progressDialog();
		}
	};

	/** Handles data for jpeg picture */
	PictureCallback jpegCallback = new PictureCallback() {
		public void onPictureTaken(byte[] data, Camera camera) {
			/* sending image */
			Log.d(TAG, "Running new thread");
						
			ProgressThread thread = new ProgressThread(data, handler);
			thread.inProgress = true;
			thread.start();
			Log.d(TAG, "onPictureTaken - jpeg");
			
			//preview.refresh();
		}
	};
	
	private void playAudio(int type) {
		try {
			myMediaPlayer = null;
			
			switch (type) {
			
			// shutter sound plays after app takes a picture		
			case Constants.SHUTTER_SOUND:
				Log.i("MCRAudioDevice", "playing camera shutter");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.shutter);
				break;
				
			// error
			case Constants.IMAGE_ERROR_SOUND:
				Log.i("MCRAudioDevice", "cannot process the image. please retake the picture");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.imageerror); 
				break;
			
			case Constants.ONE_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing one dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.one); 
				break;
			case Constants.FIVE_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing five dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.five); 
				break;
			case Constants.TEN_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing ten dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.ten); 
				break;
			case Constants.TWENTY_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing twenty dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.twenty); 
				break;
			case Constants.FIFTY_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing fifty dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.fifty); 
				break;
			case Constants.ONE_HUNDRED_DOLLAR_SOUND:
				Log.i("MCRAudioDevice", "playing hundred dollar bill");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.hundred); 
				break;
			case Constants.CONNECTION_FAILED:
				Log.i("MCRAudioDevice", "cannot connect to network");
				myMediaPlayer = MediaPlayer.create(getBaseContext(), R.raw.errornetwork); 
		        break;
			default: 
				
			}
			myMediaPlayer.setLooping(false);
			myMediaPlayer.start();
			myMediaPlayer.setOnCompletionListener(new OnCompletionListener(){
				@Override
				public void onCompletion( MediaPlayer mp ) {
					if( myMediaPlayer != null ) {
						myMediaPlayer.release();
						Log.d(TAG, "Releasing the MediaPlayer object.");
					}
				}				
			});
			
		}
		catch (Exception ex) {
		}
		
	}
	
	public native int processCurrencyImage( byte[] data, int length );
	public native int initializeCurrencyReader();
	
	private class ProgressThread extends Thread {
		protected boolean inProgress;
		private byte [] data;		//the actual image data
		private Handler handler;
		public ProgressThread(byte [] m_data, Handler m_handler) {
			data = m_data;
			handler = m_handler;
		}
		public void run() {
		

			int returnedNum;
		        
			// loop waiting sound while waiting for response from server
		    MediaPlayer mp = MediaPlayer.create(getBaseContext(), R.raw.blip);
		    mp.setLooping(true);
		    mp.start();
				
			Log.d(TAG, "START IMAGE PROCESSING!!!");
			returnedNum = processCurrencyImage( data, data.length );
	
			// Done running vision code
		    mp.stop();
		    mp.setLooping(false);
		    mp.release();
				
            // play answer
            playAudio(returnedNum);
			
            // Pass a message to the handler to let it know to restart the preview
			Message msg = handler.obtainMessage();
            Bundle b = new Bundle();
            b.putInt("running", 0);
            msg.setData(b);
            handler.sendMessage(msg);
                
            inProgress = false;
		}
		
		/*public byte[] convertJPEGtoPPM( byte[] jpeg) {
			List<Byte> ppmList = new ArrayList<Byte>();
			
			Mat jpegMat = new Mat(1, jpeg.length, CvType.CV_8U);
			jpegMat.put(0, 0, jpeg);
			
			
			Mat raw = Highgui.imdecode(jpegMat, 1);

			if( raw.empty() ) {
				Log.d(TAG, "Failed to decode jpeg image!");
				return null;
			} else {
				Log.d(TAG, "Decoded: " + raw.rows() + " x " + raw.cols() );
			}
			
			//Will encode into ppm and automatically resize it appropriately
			boolean ret = Highgui.imencode(".ppm", raw, ppmList);
			
			if( !ret ) {
				Log.d(TAG, "Failed to encode into the PPM image!");
				return null;
			} 
			if ( ppmList.isEmpty() ) {
				Log.d(TAG, "Returned encoded Byte List is empty!");
				return null;				
			}
			
			//Really should get apache commons lang to convert in one line			
			//Since ewe need to prmitive type
			byte[] ppm = new byte[ppmList.size()];
			for( int i=0; i < ppm.length; i++ ) {
				ppm[i] = ppmList.get(i).byteValue();
			}
			return ppm;
		}*/	
		
	}
	
    static {
        System.loadLibrary("MCRindep");
    }
	
}