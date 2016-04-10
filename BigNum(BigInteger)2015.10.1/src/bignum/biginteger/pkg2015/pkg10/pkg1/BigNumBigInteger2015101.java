package bignum.biginteger.pkg2015.pkg10.pkg1;

import java.io.*;
import java.util.*;
import java.math.*;
import java.util.Random;
import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;

public class BigNumBigInteger2015101 
{
    private static final int MAXLEN = 50000;
    
    public static void main(String[] args) 
    {
        int Rd;
        Random rdm = new Random(System.currentTimeMillis());
        //Scanner temp = new Scanner(System.in);
        BigInteger a,b,c;
        String str;
        
        a = null;
        b = null;

        str = "";
        for(int i = 0 ; i < MAXLEN; i++)
        {
            Rd = Math.abs(rdm.nextInt())%10;
                
            str= str + Rd;
        }
        a = new BigInteger(str);

        str = "";
        for(int i = 0 ; i < MAXLEN; i++)
        {
            Rd = Math.abs(rdm.nextInt())%10;
                
            str= str + Rd;
        }   
        b = new BigInteger(str);
        
        long start=System.currentTimeMillis();
        c = a.multiply(b);
        long end=System.currentTimeMillis();
        
        //System.out.println(a);
        //System.out.println(b);
        //System.out.println(c);
        System.out.println("Time:" + (end - start));
    }
}
