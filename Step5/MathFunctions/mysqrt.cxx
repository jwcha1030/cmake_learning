#include <iostream>
#include <cmath>

double mysqrt(double x)
{
  if (x <= 0)
  {
    return 0;
  }
  
#if defined(HAVE_LOG) && defined(HAVE_EXP)
  double result = exp(log(x) * 0.5);
  std::cout << "Computing sqrt of " << x << "to be " << result << " using log and exp" << std::endl;
#else
  double result = x;
  //Newton Raphson method: f(x)= x^2 - N where N is the number whose square root is to be obtained
  //find the root of f(x)=0.
  // Let x1=N/2, x_(n+1)=x_n - f(x_n)/f'(x_n)
  for (int i = 0; i < 10; ++i)
  {
    if (result <= 0)
      result = 0.1;
    double delta = x - (result * result);
    result = result + 0.5 * delta / result;
    std::cout << "Iteration "<< i << ": Computing sqrt of " << x << " to be " << result << std::endl;
  }
 #endif
  return result;
}
