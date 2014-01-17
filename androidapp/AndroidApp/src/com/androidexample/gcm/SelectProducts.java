package com.androidexample.gcm;

import java.util.ArrayList;
import java.util.Arrays;

import android.R.*;
import android.app.Activity;
import android.app.Dialog;
import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemLongClickListener;
import android.widget.ArrayAdapter;
import android.widget.AutoCompleteTextView;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ListView;
import android.widget.TextView;
import android.widget.Toast;

public class SelectProducts extends Activity implements OnClickListener,
        OnItemLongClickListener {

    public static MyAdapter adapter;
    private Dialog dialog;
    
	Button btnGenerateSolution;
	Button backButtonFromList;

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.select_items);
        adapter = new MyAdapter();
        ListView listView = (ListView) findViewById(R.id.list_view_add);
        listView.setAdapter(adapter);
        listView.setOnItemLongClickListener(this);
        
        btnGenerateSolution = (Button) findViewById(R.id.btnGenerateSolution);
        backButtonFromList = (Button) findViewById(R.id.backButtonFromList);
        
        btnGenerateSolution.setOnClickListener(this);
        backButtonFromList.setOnClickListener(this);

        findViewById(R.id.add_button).setOnClickListener(new OnClickListener() {

            @Override
            public void onClick(View v) {
                dialog = new Dialog(SelectProducts.this);
                dialog.setContentView(R.layout.dialog_layout);

            	AutoCompleteTextView textAutoComplete = (AutoCompleteTextView) dialog.findViewById(R.id.autocomplete_product);
            	ArrayAdapter<String> adapter2 = new ArrayAdapter<String>(SelectProducts.this, R.layout.list_item , Config.PRODUCTS);
                textAutoComplete.setAdapter(adapter2);
                dialog.findViewById(R.id.button_cancel).setOnClickListener(
                        SelectProducts.this);
                dialog.findViewById(R.id.button_ok).setOnClickListener(
                		SelectProducts.this);
                dialog.show();
            }
        });
    }
/*
    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.select_items, menu);
        return true;
    }
*/
    class MyAdapter extends BaseAdapter {

        @Override
        public int getCount() {
            // TODO Auto-generated method stub
            return Config.shopping_list.size();
        }

        @Override
        public Object getItem(int position) {
            // TODO Auto-generated method stub
            return Config.shopping_list.get(position);
        }

        @Override
        public long getItemId(int position) {
            // TODO Auto-generated method stub
            return position;
        }

        @Override
        public View getView(int position, View convertView, ViewGroup parent) {
            TextView view = (TextView) convertView;
            if (null == view) {
                view = new TextView(SelectProducts.this);
                view.setPadding(10, 10, 10, 10);
                view.setTextColor(Color.WHITE);
            }
            view.setText(Config.shopping_list.get(position));
            return view;
        }
    }

    @Override
    public void onClick(View v) {
        switch (v.getId()) {
        case R.id.backButtonFromList:
        	Intent i = new Intent(getApplicationContext(), MainActivity.class);
			startActivity(i);
			finish();
			break;
        case R.id.btnGenerateSolution:
        	if (!Config.SHOPPING_CART.isEmpty()){
	        	Intent j = new Intent(getApplicationContext(), MySurfaceViewActivity.class);
	        	
	        	int ident = getIdentOfNearestProduct();
	        	
	        	String element = Config.SHOPPING_CART.get(ident);
				Config.SHOPPING_CART.remove(ident);
				Config.SHOPPING_STACK.push(element);
				
				j.putExtra("cacat", "NU");
	        	j.putExtra("iteration", "1");
	        	j.putExtra("initx", String.valueOf(Config.posx));
	        	j.putExtra("inity", String.valueOf(Config.posy));
	        	startActivity(j);
				break;
        	}
        case R.id.button_cancel:
            dialog.dismiss();
            break;

        case R.id.button_ok:
        	String name = ((AutoCompleteTextView) dialog.findViewById(R.id.autocomplete_product)).getText().toString();
        	
        	if (!Arrays.asList(Config.PRODUCTS).contains(name)){
        		Toast.makeText(getApplicationContext(), "Product " + name + " is not a valid choice", Toast.LENGTH_LONG).show();
        		return;
        	}
        	
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
        	
        	if (Config.SHOPPING_CART.contains(text)){
        		Toast.makeText(getApplicationContext(), "Already have product:\n " + text, Toast.LENGTH_LONG).show();
        		return;
        	}
        	    	
        	Config.shopping_list.add(text);
        	Config.SHOPPING_CART.add(text);
        	dialog.dismiss();
            adapter.notifyDataSetChanged();
        
            break;
        }
    }
    
    int getIdentOfNearestProduct(){
     	double minimum_distance = 100000000;
     	int minId = 0 ;
     	
     	for (int prodId = 0; prodId < Config.SHOPPING_CART.size(); prodId++){
     		System.out.print(Config.SHOPPING_CART);
     		int posx = Integer.parseInt(Config.SHOPPING_CART.get(prodId).split("-")[1]);
     		int posy = Integer.parseInt(Config.SHOPPING_CART.get(prodId).split("-")[2]);
     		
     		float distance = (posx - Config.posx)*(posx - Config.posx) + (posy - Config.posy)*(posy - Config.posy);
     		if (distance < minimum_distance){
     			minimum_distance = distance;
     			minId = prodId;
     		}
     	}
     	return minId;
     }


    @Override
    public boolean onItemLongClick(AdapterView<?> listView, View view,
            int position, long column) {
    	String dataToBeRemoved = Config.shopping_list.get(position);
    	Config.SHOPPING_CART.remove(dataToBeRemoved);
        Config.shopping_list.remove(position);
        adapter.notifyDataSetChanged();
        return true;
    }
}