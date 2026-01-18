# Model Properties: model B <!-- omit from toc -->

In this case we will consifer the model (**B**) and assume $b(t) = const$, $\alpha(t) = const$, $\beta(t) = const$.

$$
\begin{equation}
    \dot{D}(t) = ((\alpha + \beta) \cdot b - \beta) \cdot e^{-\mu D} + r D
\end{equation}
$$

## 1. Debt equilibrium

On long dinstance races we need to keep our pace in reasonable limits, so that we should oscilate around an equilibrium ($\dot{D}(t) = 0$):

$$
\begin{equation}
    b = \dfrac{\beta}{\alpha + \beta} - \dfrac{r D}{\alpha + \beta} \cdot e^{\mu D}
\end{equation}
$$

### 1.1. Analisis of $db / d\alpha$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\alpha} = \left( r D e^{\mu D} - \beta \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

Naive intuition here is that the more dirty ($\alpha$) our development is, the more resources ($1 - b$) we have to put on debt managemnt. In other words, $db/d\alpha \le 0$:

$$
\begin{equation}
    \beta \ge r D e^{\mu D}
\end{equation}
$$

And as we can see the intuition works while the overall debt is "manageable", however when it becomes big enough, equilibrium points will move up to higher $b$ which reveals a pradox: on long distances, the mode dirty our development is, the more resources we can allocate to new development.

### 1.2. Analisis of $db / d\beta$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\beta} = \left( \alpha + r D e^{\mu D} \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

In this case, we can have a similar intuition: the more effitint out debt elimination ($\beta$) is, the more resources ($b$) we can allocate on new development ($db/d\beta \ge 0$):

$$
\begin{equation}
    \alpha + r D e^{\mu D} \ge 0
\end{equation}
$$

Since $\alpha > 0$ and $r D e^{\mu D} > 0$ for $D > 0$, this condition is always satisfied. This means that our assumption is correct.
