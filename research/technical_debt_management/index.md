# Research: technical debt as an engineering problem <!-- omit from toc -->

- [1. Mathematical model](#1-mathematical-model)
  - [1.1. Model Normalisation](#11-model-normalisation)
  - [1.2. Basic investigation of the model](#12-basic-investigation-of-the-model)
- [2. Optimal control](#2-optimal-control)

## 1. Mathematical model

To build the model we assume that:

- increasing system complexity slows team performance in a "hyperbolic" way;
- new development is somewhat dirty and creates new debt;
- existing debt or complexity creates even more complexity;
- team performance is split between new feature development and maintenance;

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = u(t) \\
        \dot{D}(t) & = α(t) \cdot u(t) - β(t) \cdot m(t) + f_D(D) \\

        u(t) & = b(t) \cdot P(t) \\
        m(t) & = (1 - b(t)) \cdot P(t) \\
        P(t) & = f_P(D) \cdot P_0 \\

        f_P(D) & \approx C_{P,0} + C_{P,1} D + O(D^2) = μ D \\
        f_D(D) & \approx C_{D,0} + C_{D,1} D + O(D^2) =   r D \\
    \end{aligned}
    \right.
\end{equation}
$$

$f_P(D)$ should follow the following rules:

- $f_P(0) = 1$: if there is no debt, we have no development penalty;
- $f_P(+\infty) = 0$: an overcomplicated system can almost not be changed at all;
- $f_P(D)$ should be smooth;

This behaviour can be modeled with the following simple functions:

|                               |                                                           |
| :---------------------------- | :-------------------------------------------------------- |
| $f_P(D) = 1 - μ D$            | the linear approximation works for a limited range of $D$ |
| $f_P(D) = \dfrac{1}{1 + μ D}$ | we will use the approximation as a case (**A**)           |
| $f_P(D) = e^{-μ D}$           | we will use the approximation as a case (**B**)           |

|   Where: |                                                                                                      |
| -------: | :--------------------------------------------------------------------------------------------------- |
|   $V(t)$ | Project value in terms of abstract story points; \[$sp$\]                                            |
|   $D(t)$ | Amount of work to fix all collected technical debt (internal, from 3rd-party dependencies); \[$sp$\] |
|   $u(t)$ | Rate of new feature development; \[$sp/t$\]                                                          |
|   $m(t)$ | Rate of maintenance development; \[$sp/t$\]                                                          |
|   $b(t)$ | Allocation factor (**control policy**); \[$1$\]                                                      |
|   $P(t)$ | Team performance; \[$sp/t$\]                                                                         |
| $f_D(D)$ | Abstract compound interest on the system complexity; \[$sp \cdot t^{-1}$\]                           |
| $f_P(D)$ | System complexity-caused performance penalty; \[$1$\]                                                |
|      $μ$ | Friction factor; \[$sp^{-1}$\]                                                                       |
|      $r$ | Debt interest rate; \[$t^{-1}$\]                                                                     |
|   $α(t)$ | New debt generation factor (development dirtiness); \[1\]                                            |
|   $β(t)$ | Debt elimination efficiency; \[1\]                                                                   |

\* sp = abstract story points.

After substitutions we have case (**A**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot \dfrac{P_0}{1 + μ D} \\
        \dot{D}(t) & = ((α(t) + β(t)) \cdot b(t) - β(t)) \cdot \dfrac{P_0}{1 + μ D} + r D \\
    \end{aligned}
    \right.
\end{equation}
$$

After substitutions we have case (**B**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot e^{-μ D} \cdot P_0 \\
        \dot{D}(t) & = ((α(t) + β(t)) \cdot b(t) - β(t)) \cdot e^{-μ D} \cdot P_0 + r D \\
    \end{aligned}
    \right.
\end{equation}
$$

### 1.1. Model Normalisation

The models are over-parametrized: $V$ and $D$ are given in abstract "amounts of work" and can be normalised using nominal performance $P_0$, while time $t \in [0, T]$ can be rewritten as $t = T \cdot τ$ with $τ \in [0, 1]$ and the following substitution:

$$
\begin{equation}
    \dfrac{dX}{dt} = \dfrac{1}{T} \dfrac{dX}{dτ} = \dfrac{d\tilde{X}}{dτ}
    \Rightarrow
    X = T \cdot \tilde{X}
\end{equation}
$$

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{\tilde{V}}(τ) & = \tilde{b}(τ) \cdot \dfrac{1}{1 + μ P_0 T \tilde{D}} \\
        \dot{\tilde{D}}(τ) &
            = ((\tilde{α}(τ) + \tilde{β}(τ)) \cdot \tilde{b}(τ) - \tilde{β}(τ)) \cdot \dfrac{1}{1 + μ P_0 T \tilde{D}}
            + r T \tilde{D}
            \\
    \end{aligned}
    \right.
\end{equation}
$$

This form reveals the same dynamics as the original system with the following substitutions:

$$
\begin{equation}
    V = P_0 T \tilde{V} ,\ \
    D = P_0 T \tilde{D} ,\ \
    \tilde{μ} = P_0 T μ ,\ \
    \tilde{r} = T r
\end{equation}
$$

- $\tilde{r}$ and $\tilde{μ}$ are technically unbounded and grow with simulation horizon $T$;
- $V$ and $D$ are given in the "nominal work" $P_0 T$ units;

After all rearrangements and removal of the tilde notation we obtain the final normalised system dynamics:

- Case (**A**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot \dfrac{1}{1 + μ D} \\
        \dot{D}(t) & = ((α(t) + β(t)) \cdot b(t) - β(t)) \cdot \dfrac{1}{1 + μ D} + r D \\
    \end{aligned}
    \right.
\end{equation}
$$

- Case (**B**):

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{V}(t) & = b(t) \cdot e^{-μ D} \\
        \dot{D}(t) & = ((α(t) + β(t)) \cdot b(t) - β(t)) \cdot e^{-μ D} + r D \\
    \end{aligned}
    \right.
\end{equation}
$$

### 1.2. Basic investigation of the model

Models:

- [Model A](<./model_props.model_A.md>)
- [Model B](<./model_props.model_B.md>)

## 2. Optimal control

Scenarios:

- [Reviews (maximum result for the given amount of time)](<./optimal_control.reviews.md>)
