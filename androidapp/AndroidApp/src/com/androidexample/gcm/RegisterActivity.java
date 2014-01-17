package com.androidexample.gcm;

import android.app.Activity;
import android.content.Intent;
import android.os.Bundle;
import android.text.InputType;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import com.google.zxing.integration.android.IntentIntegrator;
import com.google.zxing.integration.android.IntentResult;

public class RegisterActivity extends Activity implements OnClickListener{
	
	// UI elements
	EditText txtTrolleyId; 
	TextView txtMemId;
	
	// Register button
	Button btnRegister;
	Button btnSettings;
	Button btnScan;
	
	
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_register);
		
		//Get Global Controller Class object (see application tag in AndroidManifest.xml)
		final Controller aController = (Controller) getApplicationContext();
		
		// Check if Internet Connection present
		if (!aController.isConnectingToInternet()) {
			
			// Internet Connection is not present
			aController.showAlertDialog(RegisterActivity.this,
					"Internet Connection Error",
					"Please connect to working Internet connection", false);
			
			// stop executing code by return
			return;
		}

		// Check if GCM configuration is set
		if (Config.YOUR_SERVER_URL == null || Config.GOOGLE_SENDER_ID == null || Config.YOUR_SERVER_URL.length() == 0
				|| Config.GOOGLE_SENDER_ID.length() == 0) {
			
			// GCM sernder id / server url is missing
			aController.showAlertDialog(RegisterActivity.this, "Configuration Error!",
					"Please set your Server URL and GCM Sender ID", false);
			
			// stop executing code by return
			 return;
		}
		txtTrolleyId = (EditText) findViewById(R.id.txtTrolleyId);
		txtMemId = (TextView) findViewById(R.id.txtMemId);
		txtMemId.setText(Config.memId);
		
		txtTrolleyId.setInputType(InputType.TYPE_CLASS_NUMBER);
		
		btnRegister = (Button) findViewById(R.id.btnRegister);
		btnScan = (Button) findViewById(R.id.btnScan);
		btnSettings = (Button) findViewById(R.id.btnSettings);

		// Click event on Scan button
		btnSettings.setOnClickListener(this);
		// Click event on Scan button
		btnScan.setOnClickListener(this);
		// Click event on Register button
		btnRegister.setOnClickListener(this);
	}
	
	@Override
	public void onClick(View v) {
		if(v.getId()==R.id.btnScan){
			IntentIntegrator scanIntegrator = new IntentIntegrator(this);
			scanIntegrator.initiateScan();
		}
		
		if (v.getId() == R.id.btnSettings){		
			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
					
			// Launch Main Activity
			Intent i = new Intent(getApplicationContext(), Settings.class);
			
			startActivity(i);
			finish();
		}
		if (v.getId() == R.id.btnRegister){		
			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
			
			// Get data from EditText
			String trolleyId = txtTrolleyId.getText().toString(); 
			String memId = txtMemId.getText().toString();
			
			// Check if user filled the form
			if(trolleyId.trim().length() > 0 && memId.trim().length() > 0){
				
				// Launch Main Activity
				Intent i = new Intent(getApplicationContext(), MainActivity.class);
				
				// Registering user on our server					
				// Sending registraiton details to MainActivity
				i.putExtra("trolleyId", trolleyId);
				i.putExtra("memId", memId);
				startActivity(i);
				finish();
				
			}else{
				
				// user doen't filled that data
				aController.showAlertDialog(RegisterActivity.this, "Registration Error!", "Please enter your details", false);
			}
		}
	}
	
	public void onActivityResult(int requestCode, int resultCode, Intent intent) {
		//retrieve scan result
		IntentResult scanningResult = IntentIntegrator.parseActivityResult(requestCode, resultCode, intent);
		if (scanningResult != null) {
			String scanContent = scanningResult.getContents();
			String scanFormat = scanningResult.getFormatName();
			txtTrolleyId.setText(scanContent);
		}
		else{
			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
			
			aController.showAlertDialog(RegisterActivity.this,
					"No scan data received",
					"Please try again", false);
		}
	}
}
