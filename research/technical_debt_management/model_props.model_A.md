# Model Properties: model A <!-- omit from toc -->

In this case we will consifer the model (**A**) and assume $b(t) = const$, $\alpha(t) = const$, $\beta(t) = const$.

$$
\begin{equation}
    \left\{
    \begin{aligned}
        \dot{D}(t) & = \dfrac{r D \cdot (\mu D + 1) + k}{\mu D + 1} \\
        k          & = ((\alpha + \beta) \cdot b - \beta) \cdot P_0 \\
    \end{aligned}
    \right .
\end{equation}
$$

After simplifications, we are resulting with the transcendental equation which is defined for all $\dot{D}(t) \neq 0$:

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

## 1. Debt equilibrium

On long dinstance races we need to keep our pace in reasonable limits, so that we should oscilate around an equilibrium ($\dot{D}(t) = 0$):

$$
\begin{equation}
    \dfrac{r D \cdot (\mu D + 1) + ((\alpha + \beta) \cdot b - \beta)}{\mu D + 1} = 0
\end{equation}
$$

$$
\begin{equation}
    r D \cdot (\mu D + 1) + ((\alpha + \beta) \cdot b - \beta) = 0
\end{equation}
$$

$$
\begin{equation}
    b = \dfrac{\beta}{\alpha + \beta} - \dfrac{1 + \mu D}{\alpha + \beta} \cdot r D
\end{equation}
$$

### 1.1. Analisis of $db / d\alpha$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\alpha} = \left( (1 + \mu D) \cdot r D - \beta \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

Naive intuition here is that the more dirty ($\alpha$) our development is, the more resources ($1 - b$) we have to put on debt managemnt. In other words, $db/d\alpha \le 0$:

$$
\begin{equation}
    \beta \ge (1 + \mu D) \cdot r D
\end{equation}
$$

And as we can see the intuition is while the overall debt is "manageable", however when it becomes big enough, equilibrium points will move up to higher $b$ which reveals a pradox: on long distances, the dirty our code is, the resources we can allocate to a new development.

### 1.2. Analisis of $db / d\beta$

Let's write the derivative:

$$
\begin{equation}
    \dfrac{db}{d\beta} = \left( \alpha - (1 + \mu D) \cdot r D \right) \cdot \dfrac{1}{(\alpha + \beta)^2}
\end{equation}
$$

In this case, we can have a similar intuition: the more effitint out debt elimination ($\beta$) is, the more resources ($b$) we can allocate on new development ($db/d\beta \ge 0$):

$$
\begin{equation}
    \alpha \ge (1 + \mu D) \cdot r D
\end{equation}
$$

And here we see the same situation: the intuition is true only for small debts, while for bigger debts, growing refactoring effitiency will mean more resources, allocated on the debt management.
