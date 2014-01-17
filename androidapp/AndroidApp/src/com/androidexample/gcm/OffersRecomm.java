package com.androidexample.gcm;


import java.util.ArrayList;
import java.util.Collections;
import java.util.HashMap;
import java.util.Map;

import com.androidexample.gcm.OffersAll.Pair2;

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
import android.widget.Button;
import android.widget.ImageView;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class OffersRecomm extends Activity  implements OnClickListener{
	Controller aController;
	
    private ArrayList<String> datasource;
    private Dialog dialog;
    ArrayList<String> web = new ArrayList<String>();
	ArrayList<Integer> imageId = new ArrayList<Integer>();

	Map< String, Double > aa = new HashMap();
	
    
	Button back_recomm;
	Button btnOffersNear_recomm;
	Button btnOffersAll_recomm;
	CustomList adapter;
	
	@Override
	public void onCreate(Bundle savedInstanceState) { 
		super.onCreate(savedInstanceState);
		setContentView(R.layout.offers_recomm);
		
		datasource = new ArrayList<String>();
		//Get Global Controller Class object (see application tag in AndroidManifest.xml)
		final Controller aController = (Controller) getApplicationContext();
		
		// Check if Internet Connection present
		if (!aController.isConnectingToInternet()) {
			
			// Internet Connection is not present
			aController.showAlertDialog(OffersRecomm.this,
					"Internet Connection Error",
					"Please connect to working Internet connection", false);
			
			// stop executing code by return
			return;
		}
		
		back_recomm= (Button) findViewById(R.id.back_recomm);
		btnOffersAll_recomm = (Button) findViewById(R.id.btnOffersAll_recomm);
		btnOffersNear_recomm = (Button) findViewById(R.id.btnOffersNear_recomm);
		
		back_recomm.setOnClickListener(this);
		btnOffersAll_recomm.setOnClickListener(this);
		btnOffersNear_recomm.setOnClickListener(this);
		
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
		if (v.getId() == R.id.back_recomm){
			// Launch Main Activity
			Intent i = new Intent(getApplicationContext(), MainActivity.class);
			startActivity(i);
			finish();
		}
		if (v.getId() == R.id.btnOffersAll_recomm){								
			// Launch OffersAll Activity
			Intent i = new Intent(getApplicationContext(), OffersAll.class);
			startActivity(i);
		}
		if (v.getId() == R.id.btnOffersNear_recomm){		
			// Launch OffersNear Activity
			Intent i = new Intent(getApplicationContext(), OffersNear.class);
			startActivity(i);
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
	
	class Pair4 implements Comparable<Pair4>{
		String str;
		Double score;

		
		public Pair4(String str, Double score){
			this.str = str;
			this.score = score; 
		}
		@Override
		public int compareTo(Pair4 arg0) {
			Double xx = arg0.score - score;
			return xx.intValue();
		}
	}
	

	public void populateView(){

		for (String product : Config.PRODUCTS){
			aa.put(product, 0.0);	
			//Collections.sort(aa);
		}
		int numberOfProducts = 0, numberOfCalories = 0;
		for (String key : Config.PREVIOUS_SHOPPING.keySet()){
			int count = Config.PREVIOUS_SHOPPING.get(key);
			numberOfProducts += count;
			numberOfCalories += count * Integer.parseInt(Config.PRODUCTS_TO_CHARACTERISTICS.get(key).get("calories"));
			aa.put(key, aa.get(key) + count*20);
		}
		int averageNoOfCalories = numberOfCalories/numberOfProducts;
		for (String key : Config.OFFERS.keySet()){
			int valueToAdd = Integer.parseInt(Config.PRODUCTS_TO_CHARACTERISTICS.get(key).get("calories"));
			valueToAdd = averageNoOfCalories - valueToAdd;
			aa.put(key, aa.get(key) + valueToAdd);
		}
		
		for (String key : Config.OFFERS.keySet()){
			int discount = Integer.parseInt(Config.OFFERS.get(key).get("discount"));
			double valueToMultiply = (double)discount/100 + 1;
			aa.put(key, aa.get(key)*valueToMultiply);
		}
		
		ArrayList< Pair4 > bb = new ArrayList<Pair4>();
		
		for (String key : aa.keySet())
			bb.add(new Pair4(key, aa.get(key)));
		Collections.sort(bb);
		
		for (Pair4 b : bb)
		{
			String text = "";
			text = String.format("%s\nScore %.1f",  
					b.str, 
					b.score);
			text += "\n";
			if (Config.OFFERS.containsKey(b.str)){ 
				text += String.format("Discount %s",
					Config.OFFERS.get(b.str).get("discount"));
				text += "%\n";
				text += String.format("NEW Price: %s - OLD Price %s", Config.OFFERS.get(b.str).get("price"), 
						Config.PRODUCTS_TO_CHARACTERISTICS.get(b.str).get("price"));
				}
			else
				text += String.format("Price: %s", 
						Config.PRODUCTS_TO_CHARACTERISTICS.get(b.str).get("price"));

			datasource.add(text);
			web.add(text);
			imageId.add(R.drawable.gcm_logo);
		}
		
		
        adapter = new CustomList(OffersRecomm.this, web, imageId);
        ListView listView = (ListView) findViewById(R.id.list_view_recomm);
        listView.setAdapter(adapter);
        listView.setOnItemLongClickListener(new AdapterView.OnItemLongClickListener() {
        	@Override
        	public boolean onItemLongClick(AdapterView<?> parent, View view, int position, long id){
        		Toast.makeText(OffersRecomm.this, "You clicked at " + web.get(+position), Toast.LENGTH_SHORT).show();
        		
        		dialog = new Dialog(OffersRecomm.this);
                dialog.setContentView(R.layout.dialog_add_view);

                dialog.findViewById(R.id.btn_add_to_shopping_list).setOnClickListener(
                		OffersRecomm.this);
                dialog.findViewById(R.id.btn_view_in_supermarket).setOnClickListener(
                		OffersRecomm.this);
                dialog.show();
                Config.current_position = position;
                
        		return true;
        	}
        });
		adapter.notifyDataSetChanged();
	}	
}