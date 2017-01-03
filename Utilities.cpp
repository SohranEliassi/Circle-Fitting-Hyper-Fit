void RandomNormalPair( reals& x, reals& y )
/*
    Generator of pseudo-random numbers
    with standard normal distribution
    based on Box-Muller transformation
    
    "reals" can be replaced by "float", 
    "double", or "long double"; or it 
    can be predefined as one of these types
    
    Input:  none
    Output:  two real values, x and y,
    that are random, independent, and 
    have standard normal distribution 
    (with mean 0 and variance 1)
    
    Call:
    
        RandomNormalPair(x,y);
        
    Uses standard C++ random generator rand()
    
    To reseed the generator rand(), call  
    
        srand ( (unsigned)time(NULL) );
    
    before you start calling this function
    
       Nikolai Chernov, November 2011
*/
{
    reals rand1,rand2,wrand;
/*
//       version 1, by direct calculation (slower)
       
    reals pi=3.141592653589793;
    rand1 = (reals)rand()/RAND_MAX;
    rand2 = (reals)rand()/RAND_MAX;
    x = sqrt(-Two*log(rand1))*cos(Two*pi*rand2);
    y = sqrt(-Two*log(rand1))*sin(Two*pi*rand2);
*/
//       version 2, in polar form 
//         (faster and more stable numerically)

    do {
         rand1 = Two*rand()/RAND_MAX - One;
         rand2 = Two*rand()/RAND_MAX - One;
         wrand = rand1*rand1 + rand2*rand2;
       } while (wrand >= One);
    wrand = sqrt( (-Two*log(wrand) ) / wrand );
    x = rand1 * wrand;
    y = rand2 * wrand;

}

//*********************** SimulateArc ******************************

void SimulateArc(Data& data, reals a, reals b, reals R, reals theta1, reals theta2, reals sigma)
/*    
          Simulate data points equally spaced along a circular arc with Gaussian noise

  input:
          a,b         the coordinates of the center of the circle 
          R           the radius of circle
          theta1      first  endpoint of the arc (in radians)
          theta2      second endpoint of the arc (in radians)
          sigma       noise level (standard deviation of residuals)
*/
{
	int N=data.n; reals theta,dx,dy;
	
	for (int i=0; i<N; i++)
	{
		theta = theta1 + (theta2 - theta1)*i/(N-1);

//			isotropic Gaussian noise

		RandomNormalPair(dx,dy);
		data.X[i] = a + R*cos(theta) + sigma*dx;
		data.Y[i] = b + R*sin(theta) + sigma*dy;
	}
}

//********************* SimulateRandom ****************************

void SimulateRandom(Data& data, reals Window)
/*    
          Simulate data points with uniform distribution
          in the square |x|<Window, |y|<Window

  input:
          nPoints  the number of data points
*/
{
	//Data data(nPoints);

	for (int i=0; i<data.n; i++)
	{
		data.X[i] = Window*(Two*rand()/RAND_MAX - One);
		data.Y[i] = Window*(Two*rand()/RAND_MAX - One);
	}
}

//****************** Sigma ************************************
//
//   estimate of Sigma = square root of RSS divided by N
//   gives the root-mean-square error of the geometric circle fit

reals Sigma (Data& data, Circle& circle)
{
    reals sum=0.,dx,dy;

    for (int i=0; i<data.n; i++)
    {
        dx = data.X[i] - circle.a;
        dy = data.Y[i] - circle.b;
        sum += SQR(sqrt(dx*dx+dy*dy) - circle.r);
    }
    return sqrt(sum/data.n);
}

//****************** SigmaReduced ************************************
//
//   estimate of Sigma = square root of RSS divided by N
//   gives the root-mean-square error of the geometric circle fit
//
//   uses only the center of the circle (a,b), not the radius
//   the function computes the optimal radius here

reals SigmaReduced (Data& data, Circle& circle)
{
    int i,n=data.n;
    reals sum=0.,dx,dy,r,D[10];

    for (i=0; i<n; i++)
    {
        dx = data.X[i] - circle.a;
        dy = data.Y[i] - circle.b;
        D[i] = sqrt(dx*dx+dy*dy);
        sum += D[i];
    }
    r = sum/n;

    for (sum=0., i=0; i<n; i++)  sum += SQR(D[i] - r);
    
    return sqrt(sum/n);
}

//****************** SigmaReducedNearLinearCase ****************
//
//   estimate of Sigma = square root of RSS divided by N

reals SigmaReducedNearLinearCase (Data& data, Circle& circle)
{
    int i,n=data.n;
	reals a0,b0,del,s,c,x,y,z,p,t,g,W,Z;
	
	a0 = circle.a-data.meanX;  b0 = circle.b-data.meanY;
	del = One/sqrt(a0*a0 + b0*b0);
	s = b0*del;  c = a0*del;
	
	for (W=Z=0.,i=0; i<n; i++)
	{
		x = data.X[i] - data.meanX;
		y = data.Y[i] - data.meanY;
		z = x*x + y*y;
		p = x*c + y*s;
		t = del*z - Two*p;
		g = t/(One+sqrt(One+del*t));
		W += (z+p*g)/(Two+del*g);
		Z += z;
	}
	W /= n;
	Z /= n;
  
    return sqrt(Z-W*(Two+del*del*W));
}

//****************** SigmaReducedForCenteredScaled ****************
//
//   estimate of Sigma = square root of RSS divided by N

reals SigmaReducedForCenteredScaled (Data& data, Circle& circle)
{
    int i,n=data.n;
    reals sum=0.,dx,dy,r;

    for (i=0; i<n; i++)
    {
        dx = data.X[i] - circle.a;
        dy = data.Y[i] - circle.b;
        sum += sqrt(dx*dx+dy*dy);
    }
    r = sum/n;
  
    return sqrt(SQR(circle.a)+SQR(circle.b)-r*r+Two);
}

//****************** OptimalRadius ******************************
//
//     compute the optimal radius of a circle, given its center (a,b)

reals OptimalRadius (Data& data, Circle& circle)
{
    reals Mr=0.,dx,dy;

    for (int i=0; i<data.n; i++)
    {
        dx = data.X[i] - circle.a;
        dy = data.Y[i] - circle.b;
        Mr += sqrt(dx*dx + dy*dy);
    }
    return Mr/data.n;
}
