# Optimal control: Zombie Rush <!-- omit from toc -->

In this scenario we have a limited amount of time ($t \in [0; T]$) and want to minimize the gap between ideal value and acual values $V_{loss} = V_{ideal}(T) - V(T) \rightarrow min$ or $V(T) - V_{ideal} \rightarrow max$, which is equivalent to the $V(T) \rightarrow max$. Efficiency factors are considered constant: $\alpha(t) = const$, $\beta(t) = const$.

More formaly, we have the following boundary conditions:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        D(0) & = D_0 \\
        V(0) & = V_0 \\
        D(T) & = \text{free} \\
        V(T) & \rightarrow max \\
    \end{aligned}
    \right.
\end{equation}
$$

This is a Mayer's problem with $\Phi(\vec{x}(t)) = V(t)$ and $\vec{\psi}(T) = \nabla_x \Phi(\vec{x}(t))$:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        D(0) & = D_0 \\
        V(0) & = V_0 \\
        \psi_D(T) & = \dfrac{d \Phi(\vec{x}(t))}{dD} = 0 \\
        \psi_V(T) & = \dfrac{d \Phi(\vec{x}(t))}{dt} = 1 \\
    \end{aligned}
    \right.
\end{equation}
$$

## 1. Model Investigations

### 1.1. Model A

For the problem we have the following Hameltonian's function:

$$
\begin{equation}
    \begin{aligned}
        H & = \psi_V(t) \cdot b(t) \cdot \dfrac{1}{1 + \mu D} \\
          & + \psi_D(t) \cdot \left( ((\alpha + \beta) \cdot b(t) - \beta) \cdot \dfrac{1}{1 + \mu D} + r D \right) \\
    \end{aligned}
\end{equation}
$$

Which takes us the following conjugate variables $\phi$:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{\psi_V} & = -\dfrac{\partial H}{\partial V} = 0 \\
        \dot{\psi_D} &
            = -\dfrac{\partial H}{\partial V}
            = \left( \psi_V(t) \cdot b(t)
            + \psi_D(t) \cdot ((\alpha + \beta) \cdot b(t) - \beta) \right) \cdot \dfrac{\mu}{(1 + \mu D)^2}
            - \psi_D(t) \cdot r
    \end{aligned}
    \right.
\end{equation}
$$

From which we have:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        & \psi_V(t) = C_V \\
        & \psi_V(T) = 1 \\
    \end{aligned}
    \right.

    \Rightarrow

    \psi_V(t) = 1

\end{equation}
$$

Then, let's notice that our $H(\cdot)$ is linear around $b(t)$, so that we have a bang-bang optimal control function:

$$
\begin{equation}
    b(t) = \arg \max_{b(t) \in [0; 1]} H(\cdot)
\end{equation}
$$

$$
b(t)=
\begin{cases}
    1               & dH/db > 0 \\
    0               & dH/db < 0 \\
    \text{singular} & dH/db = 0 \\
\end{cases}
$$

After all substitutions we have:

$$
\begin{equation}
    \dfrac{dH}{db}
        = \left( \psi_V(t) + \psi_D(t) \cdot (\alpha + \beta) \right) \cdot \dfrac{1}{1 + \mu D}
\end{equation}
$$

$$
\begin{equation}
    \begin{aligned}
        \operatorname{sign}\dfrac{dH}{db}
            & = \operatorname{sign} (1 + \psi_D(t) \cdot (\alpha + \beta)) \\
            & = \operatorname{sign} (\psi_D(t) + \dfrac{1}{(\alpha + \beta)})
    \end{aligned}
\end{equation}
$$

$$
\begin{equation}
    b(t)= \left\{ \psi_D(t) > -\dfrac{1}{(\alpha + \beta)} \right\}
\end{equation}
$$

And as $\psi_D(T) = 0$, we have that $b(T) = 1$.

### 1.2. Model B

For the problem we have the following Hameltonian's function:

$$
\begin{equation}
    \begin{aligned}
        H & = \psi_V(t) \cdot b(t) \cdot e^{-\mu D} \\
          & + \psi_D(t) \cdot \left( ((\alpha + \beta) \cdot b(t) - \beta) \cdot e^{-\mu D} + r D \right) \\
    \end{aligned}
\end{equation}
$$

Which takes us the following conjugate variables $\phi$:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{\psi_V} & = -\dfrac{\partial H}{\partial V} = 0 \\
        \dot{\psi_D} &
            = -\dfrac{\partial H}{\partial D}
            = \mu e^{-\mu D} \left( \psi_V(t) \cdot b(t)
            + \psi_D(t) \cdot ((\alpha + \beta) \cdot b(t) - \beta) \right)
            - \psi_D(t) \cdot r
    \end{aligned}
    \right.
\end{equation}
$$

From which we have:

$$
\begin{equation}
    \left\{
    \begin{aligned}
        & \psi_V(t) = C_V \\
        & \psi_V(T) = 1 \\
    \end{aligned}
    \right.

    \Rightarrow

    \psi_V(t) = 1

\end{equation}
$$

Then, let's notice that our $H(\cdot)$ is linear around $b(t)$, so that we have a bang-bang optimal control function:

$$
\begin{equation}
    b(t) = \arg \max_{b(t) \in [0; 1]} H(\cdot)
\end{equation}
$$

$$
b(t)=
\begin{cases}
    1               & dH/db > 0 \\
    0               & dH/db < 0 \\
    \text{singular} & dH/db = 0 \\
\end{cases}
$$

After all substitutions we have:

$$
\begin{equation}
    \dfrac{dH}{db}
        = \left( \psi_V(t) + \psi_D(t) \cdot (\alpha + \beta) \right) \cdot e^{-\mu D}
\end{equation}
$$

$$
\begin{equation}
    \begin{aligned}
        \operatorname{sign}\dfrac{dH}{db}
            & = \operatorname{sign} \left( (1 + \psi_D(t) \cdot (\alpha + \beta)) \cdot e^{-\mu D} \right) \\
            & = \operatorname{sign} (1 + \psi_D(t) \cdot (\alpha + \beta)) \\
            & = \operatorname{sign} (\psi_D(t) + \dfrac{1}{(\alpha + \beta)})
    \end{aligned}
\end{equation}
$$

$$
\begin{equation}
    b(t)= \left\{ \psi_D(t) > -\dfrac{1}{(\alpha + \beta)} \right\}
\end{equation}
$$

And as $\psi_D(T) = 0$, we have that $b(T) = 1$.
