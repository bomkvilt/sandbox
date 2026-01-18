# Research: technical debt as an engineering problem <!-- omit from toc -->

- [1. Introduction](#1-introduction)
- [2. Mathematical model](#2-mathematical-model)
  - [2.1. Basic investigation of the model](#21-basic-investigation-of-the-model)
  - [2.2. Optimal control](#22-optimal-control)

## 1. Introduction

Recently I have mentioned this habr [article](<https://habr.com/ru/articles/982560/>) and got curious how technical debt management problem can be modeled as yet another optimal control problem like if it as a space craft orientation.

This article mentioned the following book "Taming Your Dragon: Addressing Your Technical Debt" which also pretended to model system dynamics in some extend.

<!-- TODO: review these references -->
After a short AI-powered research I ended up the following reference of papers:

- The canonical system-dynamics:

    – **Abdel-Hamid, “A Study of Staff Turnover, Acquisition, and Assimilation” (1989)** and later work with Madnick. These are not branded as “technical debt”, but they introduce stock-and-flow differential models where code quality degradation behaves exactly like debt accumulation. Many later authors retrofitted the term.

    – **Abdel-Hamid & Madnick, *Software Project Dynamics* (1991)**. This is the foundational book. Continuous-time differential equations for productivity, error generation, rework, and schedule pressure. Technical debt is implicit as a state variable affecting future velocity.

- Papers explicitly naming technical debt:

    – **Rios, Seaman, Guo, “Managing Technical Debt Using System Dynamics” (2018)**. This is probably the cleanest explicit DE formulation. They model principal and interest as coupled differential equations with feedback loops (maintenance effort vs. feature development).
    – **Guo, Seaman, Gomes, “Accumulation and Repayment of Technical Debt” (2011–2014 series)**. These papers introduce continuous-time accumulation models, often written as ODEs even when presented diagrammatically. Interest is a function of debt stock and maintenance rate.
    – **Brown et al., “Managing Technical Debt in Software-Reliant Systems” (2010)**. Mostly conceptual, but it introduces equations later reused in differential form by others.

- Control-theoretic and optimal control formulations:

    – **Kriebel & Ravichandran-type models adapted by Ampatzoglou et al. (circa 2015–2019)**. Here technical debt is a state variable (D(t)), development effort is a control (u(t)), and the objective minimizes cost over time. These papers often explicitly write $\dot D = \alpha u - \beta m$ with (m) maintenance effort.

    – **Izurieta et al., “Using DT and TD to Control Software Evolution”**. Hybrid discrete/continuous models; still differential in spirit.

- Econ-style continuous models:

    – **Ernst, Bellomo, Nord, “Measure It? Manage It? Ignore It?” (2015)**. Some versions include continuous growth equations for debt interest impacting delivery rate.

    – **Tom, Aurum, Vidgen, “An Exploration of Technical Debt” (2013)**. Light math, but introduces interest-rate functions later formalized as ODEs.

- Adjacent but mathematically cleaner work.

    – **Lehman’s Laws of Software Evolution** as formalized by **Belady & Lehman**. Several later papers rewrite Law II (increasing complexity) as a differential equation counteracted by refactoring effort. Technical debt is essentially a renamed complexity stock.

    – **Bankruptcy-style decay models** (e.g., decay of maintainability as an exponential function of time without refactoring). These are simple first-order linear ODEs, even when authors do not say so explicitly.

And this was quite quirious: the given models looks quite not natural because of a "magic" saturation effect stating that technical debt cannot overgrow a certain maximum level while in reality it ends up with development collapse.

In the end I have decided to unpack my unversity optimal control tools and figure out how a optimal debt control portrait looks like at last.

## 2. Mathematical model

To build the mode we assume that:

- increasing system complexity slows team performance down in a gepherbolic way;
- new development is a bit dirty and creates new debt;
- existint debt/complexity create even more complexity;
- team performance is distributed across new feature development and maintainance;

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = u(t) \\
        \dot{D}(t) & = \alpha(t) \cdot u(t) - \beta(t) \cdot m(t) + f_D(D) \\

        u(t) & = b(t) \cdot P(t) \\
        m(t) & = (1 - b(t)) \cdot P(t) \\
        P(t) & = f_P(D) \cdot P_0 \\

        f_P(D) & \approx C_{P,0} + C_{P,1} D + O(D^2) = \mu D \\
        f_D(D) & \approx C_{D,0} + C_{D,1} D + O(D^2) =   r D \\
    \end{aligned}
    \right .
\end{equation}
$$

$f_P(D)$ should follow the following rules:

- $f_P(0) = 1$: if there is no debt, we have no development penalies;
- $f_P(+\infin) = 0$: ovecomplicated system alost cannot be changed at all;
- $f_P(D)$ should be smooth;

This behaviour can be modeled with the following simple functions:

|                                 |                                                         |
| :------------------------------ | :------------------------------------------------------ |
| $f_P(D) = 1 - \mu D$            | the linear approximation works in limited ranges of $D$ |
| $f_P(D) = \dfrac{1}{1 + \mu D}$ | we will use the approximation as a case (**A**)         |
| $f_P(D) = e^{-\mu D}$           | we will use the approximation as a case (**B**)         |

|      Where: |                                                                                                      |
| ----------: | :--------------------------------------------------------------------------------------------------- |
|      $V(t)$ | Project value in terms of abstract story points; \[$sp$\]                                            |
|      $D(t)$ | Amount of work to fix all collected technical debt (internal, from 3rd-party dependencies); \[$sp$\] |
|      $u(t)$ | Race of new feature development; \[$sp/t$\]                                                          |
|      $m(t)$ | Race of maintainace development; \[$sp/t$\]                                                          |
|      $b(t)$ | Allocation factor (**control policy**); \[$1$\]                                                      |
|      $P(t)$ | Team performance; \[$sp/t$\]                                                                         |
|    $f_D(D)$ | Abstract compound interest on the system complexity; \[$sp \cdot t^{-1}$\]                           |
|    $f_P(D)$ | System complexity-caused performance penalty; \[$1$\]                                                |
|       $\mu$ | Friction factor; \[$sp^{-1}$\]                                                                       |
|         $r$ | Debt interest rate; \[$t^{-1}$\]                                                                     |
| $\alpha(t)$ | new debt generation factor (development dirtiness); \[1\]                                            |
|  $\beta(T)$ | debt elimisation efficiency; \[1\]                                                                   |

\* sp = abstract story points.

After substututions we have case (**A**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot \dfrac{P_0}{1 + \mu D} \\
        \dot{D}(t) & = ((\alpha(t) + \beta(t)) \cdot b(t) - \beta(t)) \cdot \dfrac{P_0}{1 + \mu D} + r D \\
    \end{aligned}
    \right .
\end{equation}
$$

After substututions we have case (**B**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot e^{-\mu D} \cdot P_0 \\
        \dot{D}(t) & = ((\alpha(t) + \beta(t)) \cdot b(t) - \beta(t)) \cdot e^{-\mu D} \cdot P_0 + r D \\
    \end{aligned}
    \right .
\end{equation}
$$

For futher analisys it would be more convinient operating with **dimention-less** values, so that we can make the substitions to the system above and result with the same equations:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        V_{\text{old}}(t) & = V_{\text{new}}(t) \cdot P_0 \\
        D_{\text{old}}(t) & = D_{\text{new}}(t) \cdot P_0 \\

        \mu_{\text{new}}  & = \mu_{\text{old}} \cdot P_0 \\
          r_{\text{new}}  & =   r_{\text{old}} \cdot P_0 \\
    \end{aligned}
    \right .
\end{equation}
$$

After substututions we have case (**A**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot \dfrac{1}{1 + \mu D} \\
        \dot{D}(t) & = ((\alpha(t) + \beta(t)) \cdot b(t) - \beta(t)) \cdot \dfrac{1}{1 + \mu D} + r D \\
    \end{aligned}
    \right .
\end{equation}
$$

After substututions we have case (**B**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot e^{-\mu D} \\
        \dot{D}(t) & = ((\alpha(t) + \beta(t)) \cdot b(t) - \beta(t)) \cdot e^{-\mu D} + r D \\
    \end{aligned}
    \right .
\end{equation}
$$


### 2.1. Basic investigation of the model

Models:

- [Model A](<./model_props.model_A.md>)
- [Model B](<./model_props.model_B.md>)

### 2.2. Optimal control

Optimal control strategy can be found using Pontryagin's maximum principle: 

<!-- TODO: check if this is correct -->
$$
\begin{equation}
    \left\{
    \begin{aligned}
        H(\vec{x}(t), \vec{u}(t), \vec{\psi}(t), t) &
            = \vec{\psi}(t) \cdot \vec{f}(\vec{x}(t), \vec{u}(t))
            + L(\vec{x}(t), \vec{u}(t))
        \\

        \dfrac{d \vec{\psi}}{dt} &
            = \dfrac{dH}{d \vec{\psi}}
        \\

        \vec{x}(t) & = \left\{ V(t), D(t) \right\}
        \\

        \vec{u}(t) & = \left\{ b(t) \right\}
    \end{aligned}
    \right.
\end{equation}
$$

Scenarios:

- [All In! (maximum result for the given amount of time)](<./optional_control.all_in.md>)
