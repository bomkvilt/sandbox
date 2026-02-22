# Optimal control: KPI <!-- omit from toc -->

In this scenario with proceed with the [reviews](<./optimal_control.reviews.md>) model and considering how the "greedy" feature-first strategy in several consequent periods will affect the integral result and how we can adjust the reward function to make period reward aligned with long-horizon performance goals.

For the considered dimentionless model we have these relation between original $x$ and reduced $\tilde{x}$ variables:

$$
\begin{equation}
    V = P_0 T \tilde{V} ,\ \
    D = P_0 T \tilde{D} ,\ \
    \tilde{μ} = P_0 T μ ,\ \
    \tilde{r} = T r
\end{equation}
$$

Then if we compare $k$ review periods $T$ with an integral period $T_k$ we can rescale those constants:

$$
\begin{equation}
    \tilde{V}_k = \dfrac{1}{k} \cdot\tilde{V} ,\ \
    \tilde{D}_k = \dfrac{1}{k} \cdot\tilde{D} ,\ \
    \tilde{μ}_k = k \cdot \tilde{μ} ,\ \
    \tilde{r}_k = k \cdot \tilde{r}
\end{equation}
$$

## Total value penalty
