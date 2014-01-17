import random


def getCalories(lastCategory):
    if lastCategory in ['FRUITS', 'VEGETABLES', 'HERBS']:
        return random.randint(10,50)
        return random.randint(10,50)
    if lastCategory in ['DELI', 'FROZENFOOD', 'ALCOHOL']:
        return random.randint(400,1200)
    if lastCategory in ['BAKERY', 'MEAT']:
        return random.randint(50,500)
    if lastCategory in ['GROCERY', 'CHILLERDALRY']:
        return random.randint(100,1000)
    if lastCategory in ['PERSONAL', 'HOUSEHOLD', 'BABY_TODDLER']:
        return 0
    
def getPrice(lastCategory):
    if lastCategory in ['FRUITS', 'VEGETABLES', 'HERBS']:
        return random.randint(0,4)
    if lastCategory in ['DELI', 'FROZENFOOD']:
        return random.randint(5,10)
    if lastCategory in ['BAKERY']:
        return random.randint(0,4)
    if lastCategory in ['MEAT']:
        return random.randint(7,12)
    if lastCategory in ['GROCERY', 'CHILLERDALRY']:
        return random.randint(4,15)
    if lastCategory in ['PERSONAL', 'HOUSEHOLD', 'BABY_TODDLER']:
        return random.randint(2,20)
    if lastCategory in ['ALCOHOL']:
        return random.randint(15,25)
    
def main():
        f = open('productlist.txt','r') 
        text = f.read()
        f.close()
        lines = text.splitlines()
        
        g = open('productlistStringsJava.txt', 'w')
        h = open('productlistMapCharacteristics.txt', 'w')
        hgg = open('productlistOffers.txt', 'w')
        ghh = open('previousShopping.txt', 'w')
        
        g.write("static String[] PRODUCTS = new String[] {")
        h.write("static Map<String, Map<String, String> > PRODUCTS_TO_CHARACTERISTICS = new HashMap<String, Map<String,String> >() {{")
    	hgg.write("static Map<String, Map<String, String> > OFFERS = new HashMap<String, Map<String,String> >() {{")
        ghh.write("static Map<String, Integer > OFFERS = new HashMap<String, Integer >() {{")
    
        it = 0
        
        i = 0;
        j = -10;
        for line in lines:
            line = line.strip()
            if line == line.upper():
                lastCategory = line
                j += 10
                if j == 20:
                    i += 2
                    j = 0
            else:
                it += 1
                randomNumberJ = random.randint(1, 9)
                calories = getCalories(lastCategory)
                price = getPrice(lastCategory)
                price = price + float(random.randint(0,99))/100.00
                
                posx = i;
                posy = j+randomNumberJ
                
                if it % 15 == 0:
                    g.write("\n\t")
                    h.write("\n\t")
                    hgg.write("\n\t")
                    ghh.write("\n\t")
                if it != 1:
                    g.write(",")
                g.write('"%s"' %line)
                h.write('put("%s", new HashMap<String,String>(){{ ' %line)
                numberofproducts = random.randint(0, 50)
                if numberofproducts < 20:
                    numberofproducts = 0
                ghh.write('put("%s", %d);' %(line,numberofproducts))
                h.write('put("category", "%s"); put("posx", "%d"); put("posy", "%d"); put("calories", "%d"); put("price", "%.2f");' %(lastCategory, posx, posy, calories,price))
                
                if it % 10 == 0:
					hgg.write('put("%s", new HashMap<String,String>(){{ ' %line)
					discount = random.randint(20, 80);
					hgg.write('put("category", "%s"); put("price", "%.2f"); put("discount", "%d");' %(lastCategory, price*(float(100-discount) / 100) , discount) )
					hgg.write('}});')

                h.write('}});')
                
        g.write('};')
        h.write("}};")
        hgg.write("}};");
        ghh.write("}};");
        g.close()
main()