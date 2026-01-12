# Research: technical debt as an engineering problem <!-- omit from toc -->

- [1. Introduction](#1-introduction)
- [2. Mathematical model](#2-mathematical-model)
  - [2.1. Constant allocation factor](#21-constant-allocation-factor)
    - [2.1.1. Debt equilibrium](#211-debt-equilibrium)
      - [2.1.1.1. Analisis of $db / d\\alpha$](#2111-analisis-of-db--dalpha)
      - [2.1.1.2. Analisis of $db / d\\beta$](#2112-analisis-of-db--dbeta)
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

In the project I am researching dynamics of a "physical" model, as I see it.

## 2. Mathematical model

To build the system model we assume that:

- increasing system complexity slows team performance down in a gepherbolic way;
- new development is a bit dirty and creates new debt;
- debt/complexity create even more complexity;
- team performance is distributed across new feature development and maintainance;

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = u(t) \\
        \dot{D}(t) & = \alpha u(t) - \beta m(t) + f_D(D) \\

        u(t) & = b(t) P(t) \\
        m(t) & = (1 - b(t)) P(t) \\
        P(t) & = \dfrac{P_0}{1 + f_P(D)} \\

        f_P(D) & \approx C_{P,0} + C_{P,1} D + O(D^2) = \mu D \\
        f_D(D) & \approx C_{D,0} + C_{D,1} D + O(D^2) =   r D \\
    \end{aligned}
    \right .
\end{equation}
$$

|   Where: |                                                                                                      |
| -------: | :--------------------------------------------------------------------------------------------------- |
|   $V(t)$ | Project value in terms of abstract story points; \[$sp$\]                                            |
|   $D(t)$ | Amount of work to fix all collected technical debt (internal, from 3rd-party dependencies); \[$sp$\] |
|   $u(t)$ | Race of new feature development; \[$sp/t$\]                                                          |
|   $m(t)$ | Race of maintainace development; \[$sp/t$\]                                                          |
|   $b(t)$ | Allocation factor (**control policy**); \[$1$\]                                                      |
|   $P(t)$ | Team performance; \[$sp/t$\]                                                                         |
| $f_D(D)$ | Abstract compound interest on the system complexity; \[$sp/t$\]                                      |
| $f_P(D)$ | System complexity-caused performance penalty; \[$1$\]                                                |
|    $\mu$ | Friction factor; \[$sp^{-1}$\]                                                                       |
|      $r$ | Debt interest rate; \[$1/t$\]                                                                        |

After substututions:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = \dfrac{b(t)}{1 + \mu D} P_0 \\
        \dot{D}(t) & = ((\alpha + \beta) \cdot b(t) - \beta) \dfrac{P_0}{1 + \mu D} + r D \\
    \end{aligned}
    \right .
\end{equation}
$$

### 2.1. Constant allocation factor

In this case we assume $b(t) = const$.

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{D}(t) & = \dfrac{r D \cdot (\mu D + 1) + k}{\mu D + 1} \\
        k          & = ((\alpha + \beta) \cdot b(t) - \beta) \cdot P_0 \\
    \end{aligned}
    \right .
\end{equation}
$$

After simplifications we are resulting with the trancindent equation which is defined for all $\dot{D}(t) \neq 0$:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        t_1 - t_0 & = \left(
            \dfrac{1}{2r}
                \cdot
                log \left| r D \cdot (\mu D + 1) + k \right|
            +
            \dfrac{1}{\sqrt{r} \sqrt{4 k \mu - r}}
                \cdot
                arctan \left( \dfrac{\sqrt{r} \cdot (2 D + 1)}{\sqrt{4 k \mu - r}} \right)
        \right)^{D_1}_{D_0}
        &
        \text{if: } 4 k \mu - r > 0
        \\

        t_1 - t_0 & = \left(
            \dfrac{1}{2r}
                \cdot
                log \left| r D \cdot (\mu D + 1) + k \right|
            -
            \dfrac{1}{2 \sqrt{r} \sqrt{r - 4 k \mu}}
                \cdot
                log \left(
                    \dfrac{
                        1 + \dfrac{\sqrt{r} \cdot (2 D + 1)}{\sqrt{r - 4 k \mu}}
                    }{
                        1 - \dfrac{\sqrt{r} \cdot (2 D + 1)}{\sqrt{r - 4 k \mu}}
                    }
                \right)
        \right)^{D_1}_{D_0}
        &
        \text{if: } 4 k \mu - r < 0
        \\
    \end{aligned}
    \right.
\end{equation}
$$

#### 2.1.1. Debt equilibrium

So that the system debt has a ballance point:

$$
\begin{equation}
    \dfrac{r D \cdot (\mu D + 1) + ((\alpha + \beta) \cdot b(t) - \beta) \cdot P_0}{\mu D + 1} = 0
\end{equation}
$$

$$
\begin{equation}
    r D \cdot (\mu D + 1) + ((\alpha + \beta) \cdot b(t) - \beta) \cdot P_0 = 0
\end{equation}
$$

$$
\begin{equation}
    b(t) = \dfrac{\beta}{\alpha + \beta} - \dfrac{1 + \mu D}{\alpha + \beta} \cdot \dfrac{r D}{P_0}
\end{equation}
$$

##### 2.1.1.1. Analisis of $db / d\alpha$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\alpha} =
        - \dfrac{\beta}{(\alpha + \beta)^2}
        + \dfrac{1 + \mu D}{(\alpha + \beta)^2} \cdot \dfrac{r D}{P_0}
\end{equation}
$$

Let's assume that it decreases ($db/d\alpha \le 0$):

$$
\begin{equation}
    (1 + \mu D) \cdot r D - \beta P_0 \le 0
\end{equation}
$$

$$
\begin{equation}
    \beta \ge (1 + \mu D) \cdot \dfrac{r D}{P_0}
\end{equation}
$$

Takeaways:

- $b$ decreases with $\alpha$ growth for small enough debt;
- **if debt becomes big, equilibrium point moves to higher $b$ when we increase $\alpha$**;

##### 2.1.1.2. Analisis of $db / d\beta$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\beta} =
          \dfrac{\alpha}{(\alpha + \beta)^2}
        - \dfrac{1 + \mu D}{(\alpha + \beta)^2} \cdot \dfrac{r D}{P_0}
\end{equation}
$$

Let's assume that it increases ($db/d\beta \ge 0$):

$$
\begin{equation}
    \alpha P_0 - (1 + \mu D) \cdot r D \ge 0
\end{equation}
$$

$$
\begin{equation}
    \alpha \ge (1 + \mu D) \cdot \dfrac{r D}{P_0}
\end{equation}
$$

Takeaways:

- $b$ growth with $\beta$ for small enough debts;
- **if debt becomes big, equilibrium point moves to lower $b$ when we increase $\beta$**;

### 2.2. Optimal control

In case of debt management we are interesed in project value maximisation $V(T)$ at a certain time horizon $T$.

The function can be found using Pontryagin's principal:

$$
\begin{equation}
    \begin{aligned}
        H & = \psi_V(t) \cdot \dfrac{b(t)}{1 + \mu D} P_0 \\
          & + \psi_D(t) \cdot \left( ((\alpha + \beta) \cdot b(t) - \beta) \dfrac{P_0}{1 + \mu D} + r D \right) \\
    \end{aligned}
\end{equation}
$$

Conjugate variables $\phi$:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{\psi_V} & = -\dfrac{\partial H}{\partial V} = 0 \\
        \dot{\psi_D} &
            = -\dfrac{\partial H}{\partial V}
            = \left( \psi_V(t) \cdot b(t)
            + \psi_D(t) \cdot ((\alpha + \beta) \cdot b(t) - \beta) \right) \cdot \dfrac{\mu P_0}{(1 + \mu D)^2}
            - \psi_D(t) \cdot r
    \end{aligned}
    \right.
\end{equation}
$$

$$
\begin{equation}
    \begin{aligned}
        \psi_V = C_V
    \end{aligned}
\end{equation}
$$
