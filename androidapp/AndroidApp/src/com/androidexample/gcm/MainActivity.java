package com.androidexample.gcm;

import android.app.Activity;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.AsyncTask;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;
import com.google.android.gcm.GCMRegistrar;

public class MainActivity extends Activity implements OnClickListener{
	// label to display gcm messages
	TextView lblMessage;
	Controller aController;
	
	// Asyntask
	AsyncTask<Void, Void, Void> mRegisterTask;
	
	public static String trolleyId;
	public static String memId;
	
	// Register button
	Button btnOffersNear;
	Button btnOffersRecomm;
	Button btnOffersAll;
	Button btnSupermarket;
	

	@Override
	public void onCreate(Bundle savedInstanceState) { 
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		//Get Global Controller Class object (see application tag in AndroidManifest.xml)
		aController = (Controller) getApplicationContext();
		
		// Check if Internet present
		if (!aController.isConnectingToInternet()) {
			
			// Internet Connection is not present
			aController.showAlertDialog(MainActivity.this,
					"Internet Connection Error",
					"Please connect to Internet connection", false);
			// stop executing code by return
			return;
		}
		
		// Getting trolleyId, memId from intent
		Intent i = getIntent();
		
		trolleyId = i.getStringExtra("trolleyId");
		memId = i.getStringExtra("memId");		
		
		// Make sure the device has the proper dependencies.
		GCMRegistrar.checkDevice(this);

		// Make sure the manifest permissions was properly set 
		GCMRegistrar.checkManifest(this);

		lblMessage = (TextView) findViewById(R.id.lblMessage);
		
		btnOffersNear = (Button) findViewById(R.id.btnOffersNear);
		btnOffersRecomm = (Button) findViewById(R.id.btnOffersRecomm);
		btnOffersAll = (Button) findViewById(R.id.btnOffersAll);
		btnSupermarket = (Button) findViewById(R.id.btnSupermarket);
		
		btnOffersNear.setOnClickListener(this);
		btnOffersRecomm.setOnClickListener(this);
		btnOffersAll.setOnClickListener(this);
		btnSupermarket.setOnClickListener(this);
		
		// Register custom Broadcast receiver to show messages on activity
		registerReceiver(mHandleMessageReceiver, new IntentFilter(
				Config.DISPLAY_MESSAGE_ACTION));
		
		// Get GCM registration id
		final String regId = GCMRegistrar.getRegistrationId(this);

		// Check if regid already presents
		if (regId.equals("")) {
			
			// Register with GCM			
			GCMRegistrar.register(this, Config.GOOGLE_SENDER_ID);
			
		} else {
			
			// Device is already registered on GCM Server
			if (GCMRegistrar.isRegisteredOnServer(this)) {
				
				// Skips registration.				
				Toast.makeText(getApplicationContext(), "Already registered with GCM Server", Toast.LENGTH_LONG).show();
				final Context context = this;
				mRegisterTask = new AsyncTask<Void, Void, Void>() {

					@Override
					protected Void doInBackground(Void... params) {
						
						// Register on our server
						// On server creates a new user
						aController.register(context, trolleyId, memId, regId);
						
						return null;
					}

					@Override
					protected void onPostExecute(Void result) {
						mRegisterTask = null;
					}

				};
				
				// execute AsyncTask
				mRegisterTask.execute(null, null, null);
			} else {
				
				// Try to register again, but not in the UI thread.
				// It's also necessary to cancel the thread onDestroy(),
				// hence the use of AsyncTask instead of a raw thread.
				
				final Context context = this;
				mRegisterTask = new AsyncTask<Void, Void, Void>() {

					@Override
					protected Void doInBackground(Void... params) {
						
						// Register on our server
						// On server creates a new user
						aController.register(context, trolleyId, memId, regId);
						
						return null;
					}

					@Override
					protected void onPostExecute(Void result) {
						mRegisterTask = null;
					}

				};
				
				// execute AsyncTask
				mRegisterTask.execute(null, null, null);
			}
		}
	}		
	
	@Override
	public void onClick(View v) {
		if (v.getId() == R.id.btnOffersNear){		
			// Launch OffersNear Activity
			Intent i = new Intent(getApplicationContext(), OffersNear.class);
			startActivity(i);
		}
		
		if (v.getId() == R.id.btnOffersRecomm){			
			// Launch OffersRecomm Activity
			Intent i = new Intent(getApplicationContext(), OffersRecomm.class);
			startActivity(i);
		}
		
		if (v.getId() == R.id.btnOffersAll){			
			// Launch OffersAll Activity
			Intent i = new Intent(getApplicationContext(), OffersAll.class);
			startActivity(i);
		}
		
		if (v.getId() == R.id.btnSupermarket){			
			//Intent i = new Intent(getApplicationContext(), MySurfaceViewActivity.class);
			Intent i = new Intent(getApplicationContext(), SelectProducts.class);
			startActivity(i);
		}
	}

	// Create a broadcast receiver to get message and show on screen 
	private final BroadcastReceiver mHandleMessageReceiver = new BroadcastReceiver() {
		
		@Override
		public void onReceive(Context context, Intent intent) {
			
			String newMessage = intent.getExtras().getString(Config.EXTRA_MESSAGE);
			
			// Waking up mobile if it is sleeping
			aController.acquireWakeLock(getApplicationContext());
			
			// Display message on the screen
			
			String[] xyMessage = newMessage.split(" ");
			lblMessage.append(String.format("\nLength:%d\nPosition 1%s\nPosition 2%s", xyMessage.length, xyMessage[0], xyMessage[1]));
			if (xyMessage.length == 2){
				Config.posx = Integer.parseInt(xyMessage[0])/2;
				Config.posx *= 2;
				Config.posy = Integer.parseInt(xyMessage[1]);
			}
			//lblMessage.append(newMessage + "\n");			
			
			//Toast.makeText(getApplicationContext(), "Got Message: " + newMessage, Toast.LENGTH_LONG).show();
			
			// Releasing wake lock
			aController.releaseWakeLock();
		}
	};
	
	@Override
	protected void onDestroy() {
		// Cancel AsyncTask
		if (mRegisterTask != null) {
			mRegisterTask.cancel(true);
		}
		try {
			// Unregister Broadcast Receiver
			unregisterReceiver(mHandleMessageReceiver);
			
			//Clear internal resources.
			GCMRegistrar.onDestroy(this);
			
		} catch (Exception e) {
			Log.e("UnRegister Receiver Error", "> " + e.getMessage());
		}
		super.onDestroy();
	}

}
