package com.androidexample.gcm;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class Settings extends Activity implements OnClickListener{
	
	// UI elements
	EditText txtIpAddress; 
	EditText txtMemIdSettings;
	
	// Register button
	Button btnBackRegister;
	Button btnSaveSettings;
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.settings);
		
		//Get Global Controller Class object (see application tag in AndroidManifest.xml)
		final Controller aController = (Controller) getApplicationContext();
		
		// Check if Internet Connection present
		if (!aController.isConnectingToInternet()) {
			
			// Internet Connection is not present
			aController.showAlertDialog(Settings.this,
					"Internet Connection Error",
					"Please connect to working Internet connection", false);
			
			// stop executing code by return
			return;
		}

		// Check if GCM configuration is set
		if (Config.YOUR_SERVER_URL == null || Config.GOOGLE_SENDER_ID == null || Config.YOUR_SERVER_URL.length() == 0
				|| Config.GOOGLE_SENDER_ID.length() == 0) {
			
			// GCM sernder id / server url is missing
			aController.showAlertDialog(Settings.this, "Configuration Error!",
					"Please set your Server URL and GCM Sender ID", false);
			
			// stop executing code by return
			 return;
		}
		txtIpAddress = (EditText) findViewById(R.id.txtIpAddress);
		txtMemIdSettings = (EditText) findViewById(R.id.txtMemIdSettings);
		btnBackRegister = (Button) findViewById(R.id.btnBackRegister);
		btnSaveSettings = (Button) findViewById(R.id.btnSaveSettings);
		
		txtIpAddress.setText(Config.YOUR_SERVER_URL);
		txtIpAddress.setInputType(InputType.TYPE_CLASS_TEXT);
		txtMemIdSettings.setText(Config.memId);
		txtMemIdSettings.setInputType(InputType.TYPE_CLASS_NUMBER);
		// Click event on Back button
		btnBackRegister.setOnClickListener(this);
		
		// Click event on Save button
		btnSaveSettings.setOnClickListener(this);
	}
	
	@Override
	public void onClick(View v) {
		if(v.getId()==R.id.btnBackRegister){
			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
			
			// Launch Main Activity
			Intent i = new Intent(getApplicationContext(), RegisterActivity.class);
				
			startActivity(i);
			finish();
		}
		if (v.getId() == R.id.btnSaveSettings){		
			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
			
			// Get data from EditText
			String ipAddress = txtIpAddress.getText().toString(); 
			String memId = txtMemIdSettings.getText().toString();

			if(ipAddress.trim().length() > 0){
				Config.YOUR_SERVER_URL = ipAddress;
				Intent i = new Intent(getApplicationContext(), RegisterActivity.class);
				
				startActivity(i);
				finish();
			}
			if(memId.trim().length() > 0){
				Config.memId = memId;
				Intent i = new Intent(getApplicationContext(), RegisterActivity.class);
				
				startActivity(i);
				finish();
			}
		}
	}
}