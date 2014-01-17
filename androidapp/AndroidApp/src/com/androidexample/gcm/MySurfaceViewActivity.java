package com.androidexample.gcm;
import java.util.ArrayList;
import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

import com.google.zxing.integration.android.IntentIntegrator;

import android.app.Activity;
import android.content.Intent;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.drawable.BitmapDrawable;
import android.os.Bundle;
import android.util.Pair;
import android.view.Menu;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.Toast;

public class MySurfaceViewActivity extends Activity implements OnClickListener{
     @SuppressWarnings("deprecation")
     
     Button ExitFromSupermarket;
     Button nextStep;
     Button previousStep;
     
     int posx;
     int posy;
     String name;
     
     String iteration;
     int initx;
     int inity;
     String cacat;
     
	@Override
	public void onCreate(Bundle savedInstanceState) {
	    super.onCreate(savedInstanceState);
	    setContentView(R.layout.supermarket);
	    
 		Intent i = getIntent();
 		
 		cacat = i.getStringExtra("cacat");
 		iteration = i.getStringExtra("iteration");
 		initx = Integer.parseInt(i.getStringExtra("initx"));
 		inity = Integer.parseInt(i.getStringExtra("inity"));
 		
 		String[] splittedProduct = ((String)Config.SHOPPING_STACK.peek()).split("-");
 		name = splittedProduct[0];
		posx = Integer.parseInt(splittedProduct[1]);
		posy = Integer.parseInt(splittedProduct[2]);
 		
	    ExitFromSupermarket = (Button) findViewById(R.id.ExitFromSupermarket);
	    nextStep = (Button) findViewById(R.id.nextStep);
	    previousStep = (Button) findViewById(R.id.previousStep);
	
		ExitFromSupermarket.setOnClickListener(this);
	    nextStep.setOnClickListener(this);
	    previousStep.setOnClickListener(this);
		
	    Bitmap bg = getbg();
	    View ll = (View) findViewById(R.id.supermarketMap);
	    ll.setBackgroundDrawable(new BitmapDrawable(bg));
	}
	
	public Bitmap getbg(){
		Paint paint = new Paint();
	    paint.setColor(Color.parseColor("#CD5C5C"));
	    Bitmap bg = Bitmap.createBitmap(400, 800, Bitmap.Config.ARGB_8888);
	    Canvas canvas = new Canvas(bg); 
	    for (int i = 0; i <= 800; i+=20){
	    	canvas.drawLine(0, i, 800, i, paint);
	    }
	    for (int i = 0; i <= 400; i+=20){
	    	canvas.drawLine(i, 0, i, 800, paint);
	    }
	    
	    for (int i = 0; i < 40; i++){
	    	for (int j = 0; j < 20; j++){
	    		if (Config.SUPERMARKET_MAP[i][j] == 1){
	    			canvas.drawRect(j*20, i*20, (j+1)*20, (i+1)*20, paint); 
	    		}
	    	}
	    }
	    
	    paint.setColor(Color.BLACK); 
	    paint.setStrokeWidth(5); 

	    canvas.drawText(name, min(posy*20, 300), posx*20+20, paint);
	    
	    List<Integer> trajectory = bfs(initx, inity, posx, posy);
	    for (int i = 0; i < trajectory.size()-1; i++){
	    	int first = trajectory.get(i);
	    	int second = trajectory.get(i+1);
	    	canvas.drawLine((first%100)*20+5, (first/100)*20+5, (second%100)*20+5, (second/100)*20+5, paint);
	    }
	    
	    canvas.drawCircle(inity*20+5, initx*20+5, 5, paint);
	    return bg;
	}    
	
     private float min(int i, int j) {
		if (i<j)
			return i;
		else
			return j;
	}

	public void onClick(View v) {
 		if(v.getId()==R.id.ExitFromSupermarket){
 			if (cacat.equals("DA")){
 				Config.SHOPPING_STACK.pop();
 				finish();
 				return;
 			}
 			if (!Config.SHOPPING_STACK.isEmpty()){
 				String element = (String)Config.SHOPPING_STACK.pop();
 				Config.SHOPPING_CART.add(element);
 			}
 			
			finish();
 		}
 		if(v.getId()==R.id.previousStep){
 			if (cacat.equals("DA")){
 				return;
 			}
 			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
			
			if (Integer.parseInt(iteration) == 1){
 				Toast.makeText(getApplicationContext(), "No products behind!!!", Toast.LENGTH_LONG).show();
 				return;
 			}
			
			// Launch Main Activity
			String element = (String)Config.SHOPPING_STACK.pop();
			Config.SHOPPING_CART.add(element);
			Intent i = new Intent(getApplicationContext(), MySurfaceViewActivity.class);
			
			i.putExtra("cacat", "NU");
			i.putExtra("iteration", Integer.toString(Integer.parseInt(iteration)+1));
			i.putExtra("initx", "0");
			i.putExtra("inity", "0");
			startActivity(i);
			finish();
 		}
 		if(v.getId()==R.id.nextStep){
 			if (cacat.equals("DA")){
 				return;
 			}
 			
 			if (Config.SHOPPING_CART.isEmpty()){
 				Toast.makeText(getApplicationContext(), "No products left to buy!!!", Toast.LENGTH_LONG).show();
 				return;
 			}
 			
 			//Get Global Controller Class object (see application tag in AndroidManifest.xml)
			final Controller aController = (Controller) getApplicationContext();
					
			// Launch Main Activity
			Intent i = new Intent(getApplicationContext(), MySurfaceViewActivity.class);
			
			int ident = getIdentOfNearestProduct();
        	
        	String element = Config.SHOPPING_CART.get(ident);
			Config.SHOPPING_CART.remove(ident);
			Config.SHOPPING_STACK.push(element);
			i.putExtra("cacat", "NU");
			i.putExtra("iteration", Integer.toString(Integer.parseInt(iteration)+1));
			i.putExtra("initx", Integer.toString(posx));
			i.putExtra("inity", Integer.toString(posy));
			startActivity(i);
			finish();
 		}
     }
     
     int getIdentOfNearestProduct(){
     	double minimum_distance = 100000000;
     	int minId = 0 ;
     	
     	for (int prodId = 0; prodId < Config.SHOPPING_CART.size(); prodId++){
     		String[] splittedProduct = Config.SHOPPING_CART.get(prodId).split("-");
     		int posx = Integer.parseInt(splittedProduct[1]);
     		int posy = Integer.parseInt(splittedProduct[2]);
     		
     		float distance = (posx - initx)*(posx - initx) + (posy - inity)*(posy - inity);
     		if (distance < minimum_distance){
     			minimum_distance = distance;
     			minId = prodId;
     		}
     	}
     	return minId;
     }
     

	 List<Integer> bfs(int xi, int yi, int xf, int yf){
		int[][] tata = new int[40][20]; 
		int[][] value = new int[40][20];
		boolean[][] visited = new boolean[40][20]; 
		 
		int[] xplus = {1, -1, 0, 0};
		int[] yplus = {0, 0, -1, 1};
		 
		// BFS uses Queue data structure
		Queue<Integer> queue = new LinkedList<Integer>();
		queue.add(xi*100+yi);
		visited[xi][yi] = true;
		List<Integer> result = new ArrayList<Integer>();
		
		
		while(!queue.isEmpty() && tata[xf][yf] == 0) {
			int node = (Integer)queue.remove();
			int parentx = node/100;
			int parenty = node%100;
			for (int i = 0; i < 4; i++) {
				int newx = parentx + xplus[i]; 
				int newy = parenty + yplus[i];
				if (newx >=0 && newy >=0 && newx < 40 && newy < 20){ 
					if (!visited[newx][newy] && Config.SUPERMARKET_MAP[newx][newy] == 0){
						visited[newx][newy]=true;
						queue.add(newx*100+newy);
						value[newx][newy] = value[parentx][parenty] + 1;
						tata[newx][newy] = parentx*100 + parenty;
					}
				}
			}
		}
		
		result.add(xf*100+yf);

		if (tata[xf][yf] == 0){
			if (tata[xf+1][yf] != 0) tata[xf][yf] = (xf+1)*100+yf;
			else if (tata[xf][yf+1] != 0) tata[xf][yf] = (xf)*100+(yf+1);
			else if (tata[xf-1][yf] != 0) tata[xf][yf] = (xf-1)*100+yf;
			else if (tata[xf][yf-1] != 0) tata[xf][yf] = (xf)*100+(yf-1);
			
		}
		while (value[xf][yf] != 0){
			int taticu = tata[xf][yf];
			result.add(taticu);
			xf = taticu/100;
			yf = taticu%100;
		}
		return result;
	 };
}