#define __libmd_src_file__
#ifndef libmd_h
#include "../libmd.h"
#endif

template<class X> X COULOMB(X r,std::vector<ldf> &parameters)
{
    //!
    //! Coulomb potential:
    //! \f[V_{\text{COULOMB}}(r)=\frac{q}{r}\f] <br>
    //! This function depends on one parameter:
    //! <ul>
    //! <li> The charge coupling between two partilces: \f$q\f$ </li>
    //! </ul>
    //!
    const ldf q=parameters[0];
    return q/r;
}

template<class X> X YUKAWA(X r,std::vector<ldf> &parameters)
{
    //!
    //! Yukawa potential:
    //! \f[V_{\text{YUKAWA}}(r)=\frac{b}{r e^{kr}}\f] <br>
    //! This function depends on two parameters:
    //! <ul>
    //! <li> the coupling strength between two partilces \f$b\f$ </li>
    //! <li> the Yukawa reciprocal length scale \f$k\f$ </li>
    //! </ul>
    //!
    const ldf b=parameters[0];
    const ldf k=parameters[1];
    return b/(r*exp(k*r));
}

template<class X> X HOOKEAN(X r,std::vector<ldf> &parameters)
{
    //!
    //! Hookian potential (Harmonic spring potential):
    //! \f[V_{\text{HOOKEAN}}(r)=\tfrac{1}{2}k{(r-r_0)}^2\f] <br>
    //! This function depends on two parameters:
    //! <ul>
    //! <li> the spring constant \f$k\f$ </li>
    //! <li> the spring's rest length \f$r_0\f$ </li>
    //! </ul>
    //!
    const ldf k=parameters[0];
    const ldf r0=parameters[1];
    return k/2.0*pow(r-r0,2);
}

template<class X> X LJ(X r,std::vector<ldf> &parameters)
{
    //!
    //! The famous Lenard-Jones potential:
    //! \f[V_{\text{LJ}}(r)=4 \epsilon \left({\left( \frac{\sigma}{r} \right)}^{12} - {\left( \frac{\sigma}{r} \right)}^6 \right) \f] <br>
    //! This function depends on two parameters:
    //! <ul>
    //! <li> the coupling constant \f$\epsilon\f$ </li>
    //! <li> the characteristic length scale \f$\sigma\f$ </li>
    //! </ul>
    //!
    const ldf e=parameters[0];
    const ldf s=parameters[1];
    return 4.0*e*(pow(s/r,12)-pow(s/r,6));
}

template<class X> X MORSE(X r,std::vector<ldf> &parameters)
{
    //!
    //! Morse potential:
    //! \f[V_{\text{MORSE}}(r)=d{\left(1-e^{a(r_e-r)}\right)}^2\f] <br>
    //! This function depends on three parameters:
    //! <ul>
    //! <li> the dissociation energy \f$d\f$ </li>
    //! <li> the width \f$a\f$ </li>
    //! <li> the equilibrium bond distance \f$r_e\f$ </li>
    //! </ul>
    //!
    const ldf d=parameters[0];
    const ldf a=parameters[1];
    const ldf re=parameters[2];
    return d*pow(1.0-exp(a*(re-r)),2);
}

template<class X> X FORCEDIPOLE(X r,std::vector<ldf> &parameters)
{
    // exerts a constant force f = parameters[0]. Positive force => extension of dipole
    const ldf f = parameters[0];
    return -f*r;
}

template<class X> X HOOKEANFORCEDIPOLE(X r,std::vector<ldf> &parameters)
{
    std::vector<ldf> sprparams(parameters.begin(),parameters.begin()+2);
    std::vector<ldf> fdparams(parameters.begin()+2,parameters.begin()+3);

    if (parameters.size() == 3) return HOOKEAN(r, sprparams) + FORCEDIPOLE(r, fdparams);

    // if threshold exists: force dipole kicks in when force due to spring extension/compression is larger than threshold.
    // positive f => threshold is in extension; negative f => threshold is in compression.
    // threshold must be positive for this interpretation to hold.
    const ldf threshold = parameters[3];
    return HOOKEAN(r, sprparams) + (sprparams[0]*(r-sprparams[1])*fdparams[0]/std::abs(fdparams[0]) > threshold)*FORCEDIPOLE(r, fdparams);
}

template<class X> X ANHARMONICSPRING(X r,std::vector<ldf> &parameters)
{
    //!
    //! Anharmoninc spring:
    //! \f[V_{\text{ANHARMONICSPRING}}(r)=\tfrac{k}{\alpha}{\lvert r-r_0 \rvert}^{\alpha}\f] <br>
    //! This function depends on three parameters:
    //! <ul>
    //! <li> the 'spring' constant \f$k\f$ </li>
    //! <li> the 'spring' rest length \f$r_0\f$ </li>
    //! <li> the exponent \f$\alpha\f$ </li>
    //! </ul>
    //!
    const ldf k=parameters[0];
    const ldf r0=parameters[1];
    const ldf alpha=parameters[2];
    return (k/alpha)*pow(abs(r-r0),alpha);
}
