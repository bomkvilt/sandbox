# Model Properties: model B <!-- omit from toc -->

In this case we consider the model (**B**) and assume $b(t) = \text{const}$, $\alpha(t) = \text{const}$, $\beta(t) = \text{const}$.

$$
\begin{equation}
    \dot{D}(t) = ((\alpha + \beta) \cdot b - \beta) \cdot e^{-\mu D} + r D
\end{equation}
$$

## 1. Debt equilibrium

On long time horizons we need to keep the pace within reasonable limits, so we should oscillate around an equilibrium ($\dot{D}(t) = 0$):

$$
\begin{equation}
    b = \dfrac{\beta}{\alpha + \beta} - \dfrac{r D}{\alpha + \beta} \cdot e^{\mu D}
\end{equation}
$$

### 1.1. Analysis of $db / d\alpha$

The derivative is:

$$
\begin{equation}
    \dfrac{db}{d\alpha} = \left( r D e^{\mu D} - \beta \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

A naive intuition is that the dirtier ($\alpha$) our development is, the more resources ($1 - b$) we must put into debt management. In other words, $db/d\alpha \le 0$:

$$
\begin{equation}
    \beta \ge r D e^{\mu D}
\end{equation}
$$

As we can see, this intuition holds only while the overall debt is "manageable"; when it becomes large enough, equilibrium points move to higher $b$, which reveals a paradox: over long horizons, the dirtier our development is, the more resources we can allocate to new development.

### 1.2. Analysis of $db / d\beta$

The derivative is:

$$
\begin{equation}
    \dfrac{db}{d\beta} = \left( \alpha + r D e^{\mu D} \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

Here we have a similar intuition: the more efficient our debt elimination ($\beta$) is, the more resources ($b$) we can allocate to new development ($db/d\beta \ge 0$):

$$
\begin{equation}
    \alpha + r D e^{\mu D} \ge 0
\end{equation}
$$

Since $\alpha > 0$ and $r D e^{\mu D} > 0$ for $D > 0$, this condition is always satisfied. This means that our assumption is correct.
