using Microsoft.EntityFrameworkCore.Migrations;

#nullable disable

namespace Server.Migrations
{
    public partial class InitialCreate : Migration
    {
        protected override void Up(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.CreateTable(
                name: "SUSTest",
                columns: table => new
                {
                    Id = table.Column<int>(type: "INTEGER", nullable: false)
                        .Annotation("Sqlite:Autoincrement", true),
                    Question = table.Column<string>(type: "TEXT", nullable: true),
                    StronglyDisagree = table.Column<bool>(type: "INTEGER", nullable: false),
                    Disagree = table.Column<bool>(type: "INTEGER", nullable: false),
                    Neutral = table.Column<bool>(type: "INTEGER", nullable: false),
                    Agree = table.Column<bool>(type: "INTEGER", nullable: false),
                    StronglyAgree = table.Column<bool>(type: "INTEGER", nullable: false)
                },
                constraints: table =>
                {
                    table.PrimaryKey("PK_SUSTest", x => x.Id);
                });
        }

        protected override void Down(MigrationBuilder migrationBuilder)
        {
            migrationBuilder.DropTable(
                name: "SUSTest");
        }
    }
}
