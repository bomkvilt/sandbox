import click


@click.group()
def cli():
    pass


@cli.command()
def smartthings():
    click.echo("smartthings")


if __name__ == "__main__":
    cli()
