package com.androidexample.gcm;

import java.util.ArrayList;
import java.util.Collections;

import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class OffersNear extends Activity implements OnClickListener{
		
    private ArrayList<String> datasource;
    private Dialog dialog;
    ArrayList<String> web = new ArrayList<String>();
	ArrayList<Integer> imageId = new ArrayList<Integer>();
	
	// Register button
	Button back_near;
	Button btnOffersRecomm_near;
	Button btnOffersAll_near;
	CustomList adapter;

	@Override
	public void onCreate(Bundle savedInstanceState) { 
		super.onCreate(savedInstanceState);
		setContentView(R.layout.offers_near);
		
		datasource = new ArrayList<String>();        
		//Get Global Controller Class object (see application tag in AndroidManifest.xml)
		final Controller aController = (Controller) getApplicationContext();
		
		// Check if Internet Connection present
		if (!aController.isConnectingToInternet()) {
			
			// Internet Connection is not present
			aController.showAlertDialog(OffersNear.this,
					"Internet Connection Error",
					"Please connect to working Internet connection", false);
			
			// stop executing code by return
			return;
		}
		
		// Check if GCM configuration is set
		if (Config.YOUR_SERVER_URL == null || Config.GOOGLE_SENDER_ID == null || Config.YOUR_SERVER_URL.length() == 0
				|| Config.GOOGLE_SENDER_ID.length() == 0) {
			
			// GCM sernder id / server url is missing
			aController.showAlertDialog(OffersNear.this, "Configuration Error!",
					"Please set your Server URL and GCM Sender ID", false);
			
			// stop executing code by return
			 return;
		}
		
		back_near = (Button) findViewById(R.id.back_nearest);
		btnOffersRecomm_near = (Button) findViewById(R.id.btnOffersRecomm_nearest);
		btnOffersAll_near = (Button) findViewById(R.id.btnOffersAll_nearest);
		
		back_near.setOnClickListener(this);
		btnOffersRecomm_near.setOnClickListener(this);
		btnOffersAll_near.setOnClickListener(this);
		
		populateView();
	}		
	

	private class CustomList extends ArrayAdapter<String>{
		private final Activity context;
		private final ArrayList<String> web;
		private final ArrayList<Integer> imageId;
		
		public CustomList(Activity context, ArrayList<String> web2, ArrayList<Integer> imageId2){
			super(context, R.layout.list_single, web2);
			this.context = context;
			this.web = web2; 
			this.imageId = imageId2;
		}

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
        	LayoutInflater inflater = context.getLayoutInflater();
        	View rowView = inflater.inflate(R.layout.list_single, null, true);
            TextView txtTitle = (TextView) rowView.findViewById(R.id.txxxt);
            
            ImageView imageView = (ImageView) rowView.findViewById(R.id.imggg);
            txtTitle.setText(web.get(position));
            
            imageView.setImageResource(imageId.get(position));
            return rowView;
        }
    }
	
	@Override
	public void onClick(View v) {
		if (v.getId() == R.id.btnOffersAll_nearest){
			// Launch OffersNear Activity
			Intent i = new Intent(getApplicationContext(), OffersAll.class);
			startActivity(i);
			// Registering user on our server					
		}
		if (v.getId() == R.id.btnOffersRecomm_nearest){								
			// Launch OffersRecomm Activity
			Intent i = new Intent(getApplicationContext(), OffersRecomm.class);
			startActivity(i);
			// Registering user on our server					
		}
		if (v.getId() == R.id.back_nearest){		
			// Launch OffersAll Activity
			Intent i = new Intent(getApplicationContext(), MainActivity.class);
			startActivity(i);
			// Registering user on our server					
			finish();
		}
		if (v.getId() == R.id.btn_view_in_supermarket){
    		String name = web.get(+Config.current_position).split("\n")[0];
    		
    		String category = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("category");
        	String posx = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("posx");
        	String posy = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("posy");
        	
        	String price = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("price");
        	String calories = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("calories");
        	
        	String text = name;
        	text = text.concat("-"); text = text.concat(posx);
        	text = text.concat("-"); text = text.concat(posy);
        	text = text.concat("-"); text = text.concat(category);
        	text = text.concat("-"); text = text.concat(price); text = text.concat(" pounds");	
        	text = text.concat("-"); text = text.concat(calories); text = text.concat(" cals");
        	
        	
			Config.SHOPPING_STACK.push(text);
			Intent j = new Intent(getApplicationContext(), MySurfaceViewActivity.class);
			j.putExtra("cacat", "DA");
			j.putExtra("iteration", "1");
        	j.putExtra("initx", String.valueOf(Config.posx));
        	j.putExtra("inity", String.valueOf(Config.posy));
        	startActivity(j);
        	dialog.dismiss();
		}
		if (v.getId() == R.id.btn_add_to_shopping_list){
			String name = web.get(+Config.current_position).split("\n")[0];
			
			String category = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("category");
        	String posx = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("posx");
        	String posy = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("posy");
        	String price;
        	if (Config.OFFERS.containsKey(name))
        		price = Config.OFFERS.get(name).get("price");
        	else
        		price = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("price");
        	String calories = Config.PRODUCTS_TO_CHARACTERISTICS.get(name).get("calories");
        	
        	String text = name; 
        	text = text.concat("-"); text = text.concat(posx);
        	text = text.concat("-"); text = text.concat(posy);
        	text = text.concat("-"); text = text.concat(category);
        	text = text.concat("-"); text = text.concat(price); text = text.concat(" pounds");	
        	text = text.concat("-"); text = text.concat(calories); text = text.concat(" cals");
        	
        	if (Config.SHOPPING_CART.contains(text)){
        		Toast.makeText(getApplicationContext(), "Already have product:\n " + text, Toast.LENGTH_LONG).show();
        		return;
        	}
        	
        	Config.shopping_list.add(text);
        	Config.SHOPPING_CART.add(text);
        	dialog.dismiss();
		}	

	}
	
	class Pair3 implements Comparable<Pair3>{
		String str;
		Integer posx;
		Integer posy;

		public Pair3(String str, String posx, String posy){
			this.str = str;
			this.posx = Integer.parseInt(posx);
			this.posy = Integer.parseInt(posy);
		}
		
		public float distance(){
			return (float) Math.sqrt((posx -Config.posx) *(posx-Config.posx)+
					(posy -Config.posy) *(posy-Config.posy));
		}
		
		@Override
		public int compareTo(Pair3 arg0) {
			return (int) (distance() - arg0.distance());
		}

	}
	
	public void populateView(){
		ArrayList< Pair3 > aa = new ArrayList<Pair3>();
		
		for (String product : Config.OFFERS.keySet())
			aa.add(new Pair3(
					product, 
					Config.PRODUCTS_TO_CHARACTERISTICS.get(product).get("posx"),
					Config.PRODUCTS_TO_CHARACTERISTICS.get(product).get("posy")));	
		Collections.sort(aa);
		
		for (Pair3 b : aa){
			String text = "";
			text = String.format("%s\nDistance %.2f meters \nDiscount: %s", 
					b.str, 
					b.distance(), 
					Config.OFFERS.get(b.str).get("discount"));
			text += "%\n";
			text += String.format("NEW Price %s", 
					Config.PRODUCTS_TO_CHARACTERISTICS.get(b.str).get("price"));

			datasource.add(text);
			web.add(text);
			imageId.add(R.drawable.gcm_logo);
		}
		
		adapter = new CustomList(OffersNear.this, web, imageId);
        ListView listView = (ListView) findViewById(R.id.list_view_near);
        listView.setAdapter(adapter);
        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
        	@Override
        	public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id){
        		Toast.makeText(OffersNear.this, "You clicked at " + web.get(+position), Toast.LENGTH_SHORT).show();

        		dialog = new Dialog(OffersNear.this);
                dialog.setContentView(R.layout.dialog_add_view);

                dialog.findViewById(R.id.btn_add_to_shopping_list).setOnClickListener(
                		OffersNear.this);
                dialog.findViewById(R.id.btn_view_in_supermarket).setOnClickListener(
                		OffersNear.this);
                dialog.show();
                Config.current_position = position;

        		return true;
        	}
        });
		adapter.notifyDataSetChanged();
	}
}
